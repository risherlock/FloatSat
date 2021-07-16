
#include "rodos.h"

#include "lwip/sys.h"
#include "lwip/init.h"
#include "lwip/timers.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#include "netif/etharp.h"
#include "rodos-lwip.h"


extern "C"{

err_t rodosnetif_init(struct netif* netif);
err_t rodosnetif_link_output(struct netif *netif,struct pbuf *p);


u32_t sys_now(){
	int64_t millis = NOW() / MILLISECONDS;
	return millis;
}

void rodos_lwip_error_print(const char* format, const char* msg,int line,const char* file){
	if(file ==0){
		xprintf(format,msg);
	}else{
		xprintf(format,msg,line,file);
	}
}

err_t rodosnetif_init(struct netif* netif){
	IPStack* ipstack = (IPStack*)netif->state;

	netif->output= &etharp_output;
	netif->linkoutput = &rodosnetif_link_output;
	netif->hwaddr_len=6;
	memcpy(netif->hwaddr,ipstack->eth->getMACAddr(),6);
	netif->mtu=1500;
	netif->name[0]= 'e';
	netif->name[1]= 'n';
	netif->num=0;
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP ;
	return ERR_OK;
}

err_t rodosnetif_link_output(struct netif *netif,struct pbuf *p){
	IPStack* ipstack = (IPStack*)netif->state;
	ETH_ERR result;

	do{
		result=ipstack->eth->write(p);
		switch(result){
		case ETH_ERR_OK:
			break;
		case ETH_ERR_BUFFERFULL:
			ipstack->eth->suspendUntilWriteFinished(100*MILLISECONDS);
			break;

		case ETH_ERR_LINKDOWN:
			return ERR_RTE;
		case ETH_ERR_UNKNOWN:
		default:
			return ERR_IF;
		}

	}while(result != ETH_ERR_OK);

	return ERR_OK;
}

}


//This must be moved to IPStack class, to support more then 1 interface
netif rodosnetif;
IPStack* IPStack::instance;



IPStack::IPStack(HAL_ETH* _eth,IPAddr* _ip,IPAddr* _mask,IPAddr* _gw): StaticThread<>("TCP/IP Thread", 400){
	if(instance) RODOS_ERROR("Only one IP Stack is allowed");

	eth=_eth;
	instance=this;
	linkstatus=false;
	ipstackInitialized=false;

	if(_ip){
		ip=*_ip;
		if(_mask){
			mask=*_mask;
		}else{
			mask.a=255;mask.b=255;mask.c=255;mask.d=0;
		}
		if(_gw){
			gw=*_gw;
		}else{
			gw.a=0;gw.b=0;gw.c=0;gw.d=0;
		}
		dhcp=false;
	}else{
		dhcp=true;
	}
}

void IPStack::ensureInit() {
	if(ipstackInitialized) return;

	eth->init();
	lwip_init();

	ip_addr_t _ip,_mask,_gw;
	IP4_ADDR(&_ip, ip.a,ip.b,ip.c,ip.d);
	IP4_ADDR(&_mask, mask.a,mask.b,mask.c,mask.d);
	IP4_ADDR(&_gw, gw.a,gw.b,gw.c,gw.d);

	netif_add(&rodosnetif,&_ip,&_mask,&_gw,this,&rodosnetif_init,&ethernet_input);
	netif_set_default(&rodosnetif);
	netif_set_up(&rodosnetif);

	ipstackInitialized=true;
}

void IPStack::run(){
	uint32_t msUntilNextTimeout=1;
	int64_t lastLinkCheckTime=0;

  ensureInit();

	while(true){

		eth->suspendUntilDataReady(NOW()+msUntilNextTimeout*MILLISECONDS);

		IPsem.enter();
			if(linkstatus){
				pbuf* recvFrame;
				switch(eth->read(&recvFrame)){
					case ETH_ERR_OK:
						rodosnetif.input(recvFrame,&rodosnetif);
						break;
					case ETH_ERR_LINKDOWN:
						lastLinkCheckTime -= LINK_CHECK_INTERVAL; // check for linkstatus now
						break;
					case ETH_ERR_FRAMEINVALID:
						break;
					case ETH_ERR_UNKNOWN:
						break;
					case ETH_ERR_NODATA:
						break;
					default:
		                break;
				}
			}

			int64_t n= NOW();
			if(lastLinkCheckTime + LINK_CHECK_INTERVAL < n){
				lastLinkCheckTime=n;

				bool newStatus= eth->checkLinkStatus();
				if(newStatus != linkstatus){
					linkstatus = newStatus;

					//link went up/down
					//start/stop interface
					if(linkstatus){
						PRINTF("NET: Link is up\n");
						if(dhcp){
							dhcp_start(&rodosnetif);
							PRINTF("NET:Sending DHCP request\n");
						}else{
							netif_set_up(&rodosnetif);
						}
					}else{
						PRINTF("NET:Link is down\n");
						if(dhcp) dhcp_stop(&rodosnetif);
						netif_set_down(&rodosnetif);
					}

				}
			}

			msUntilNextTimeout = sys_check_timeouts();
			if(msUntilNextTimeout > 2000) msUntilNextTimeout = 2000;
		IPsem.leave();
	}
}
