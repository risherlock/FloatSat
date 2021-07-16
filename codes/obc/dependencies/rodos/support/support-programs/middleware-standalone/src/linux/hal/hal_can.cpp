/*
 * hal_can.cpp
 *
 *  Created on: 04.06.2013
 *      Author: Erik
 */

//For Linux SocketCAN API

#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include <sys/socket.h>
#include<sys/stat.h>
#include <sys/types.h>

#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <netdb.h>
// #include <netinet/in.

#include "hal/hal_can.h"
#include "rodos-middleware.h"
#include "fifo.h"
#include "hw_udp.h"



#include <new>


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

#define MAX_FILTERS 20

//================================================================================
//Mapping of CAN IDs to linux can network device names
#define MAX_CAN_DEVICES 4
const char *canDeviceNames[MAX_CAN_DEVICES] = {"can0","can1","vcan0","vcan1"};
//================================================================================

#define MAX_HALCAN_INSTANCES 16
int numHalCanInstances = 0;
HW_HAL_CAN* activeDevices[MAX_HALCAN_INSTANCES];



void CANGlobalInit(){
	static bool init=false;
	if(!init){
		init=true;




	}

}



#define MAX_FILTERS 20

struct CAN_Filter{
	uint32_t canID;
	uint32_t canIDmask;
	bool isExtID;
	bool rtr;
	HAL_CAN* owner;
	HW_HAL_CAN* owner_context;
};





class HW_HAL_CAN {
	public:

	const char* devName;
	int s;

	can_filter filters[MAX_FILTERS];
	int numFilters;

	Fifo<can_frame,64> RxFifo;
	volatile bool rxFifoEmpty;

	HAL_CAN* hal_can;



	public:
	HW_HAL_CAN();

	void setupFilters();



};






HW_HAL_CAN::HW_HAL_CAN(){

}

void HW_HAL_CAN::setupFilters(){

	if(numFilters > 0){
		setsockopt(s,SOL_CAN_RAW,CAN_RAW_FILTER,filters,sizeof(can_filter)*numFilters);
	}else{
		setsockopt(s,SOL_CAN_RAW,CAN_RAW_FILTER,NULL,0);
	}


}




HAL_CAN::HAL_CAN(CAN_IDX canIdx, GPIO_PIN rxPin, GPIO_PIN txPin){
	context= new HW_HAL_CAN(); //we are on linux so new is availible
	context->devName = canDeviceNames[canIdx];
	context->rxFifoEmpty=true;
	context->s=0;
	context->hal_can=this;
	if( numHalCanInstances >= MAX_HALCAN_INSTANCES){
		ERROR("Too many HAL_CAN instances, see hal_can.cpp: #define MAX_HALCAN_INSTANCES\n");
		return;
	}
	activeDevices[numHalCanInstances]= context;
	numHalCanInstances++;
}

int HAL_CAN::init(unsigned int baudrate){

	if(context->s > 0){
		close(context->s);
	}

  struct sockaddr_can addr;
  struct ifreq ifr;

	if((context->s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
			return -1;
			PRINTF("socket fail\n");
	}

	strcpy(ifr.ifr_name, context->devName);
	ioctl(context->s, SIOCGIFINDEX, &ifr);
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if(bind(context->s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		return -1;
		PRINTF("bind fail\n");
	}

	init_sigio_handler();

	fcntl(context->s,F_SETFL, O_NONBLOCK);
	fcntl(context->s, F_SETOWN, getpid());
	    /* turn on async mode -- this is the important part which enables signal delivery */
	fcntl(context->s, F_SETFL, fcntl(context->s, F_GETFL, 0) | O_ASYNC);

	context->setupFilters();

	return 0;


}

void HAL_CAN::reset(){
	close(context->s);
	context->s=0;
}

int HAL_CAN::config(CAN_PARAMETER_TYPE type, int paramVal){
	switch(type){
	case CAN_PARAMETER_BAUDRATE:

	return -1;


	}
	return -1;
}


int  HAL_CAN::status(CAN_STATUS_TYPE type){

	switch(type){
	case CAN_STATUS_RX_ERROR:
		return -1;
	case CAN_STATUS_TX_ERROR:

		return -1;

	case CAN_STATUS_RX_LEVEL:
		return -1;
	}
	return -1;

}


bool  HAL_CAN::isWriteFinished(){
//	return true;
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(context->s,&fdset);

	timeval tval;
	tval.tv_sec=0;
	tval.tv_usec=0;

	// check if  fd can be written
	if(select(context->s+1,0,&fdset,0,&tval) > 0)
		return true;
	else
		return false;
}

bool  HAL_CAN::isDataReady(){
	return !context->rxFifoEmpty;
}

bool  HAL_CAN::addIncomingFilter(uint32_t ID, uint32_t IDMask, bool extID, bool rtr){

	if(context->numFilters >= MAX_FILTERS)
		return false;


	context->filters[context->numFilters].can_id =    (extID ? (ID & 0x1FFFFFFF) : (ID & 0x7FF)) & ~CAN_ERR_FLAG;
	context->filters[context->numFilters].can_mask=  ~(extID ? (IDMask) : (IDMask | 0xFFFFF800));
	context->filters[context->numFilters].can_mask |= 0xE0000000;
	context->filters[context->numFilters].can_mask &= ~(CAN_ERR_FLAG);

	context->filters[context->numFilters].can_id |= (rtr ? CAN_RTR_FLAG :0) | (extID ? CAN_EFF_FLAG :0);

	context->numFilters++;

	context->setupFilters();

	return true;
}




int HAL_CAN::write(const char* sendBuf, int len,uint32_t canID,  bool extID, bool rtr){



	can_frame f;
	if(len > 8) return -1;

	f.can_id  = extID ? (canID & 0x1FFFFFFF ): (canID & 0x7FF);
	f.can_id |= (rtr ? CAN_RTR_FLAG :0) | (extID ? CAN_EFF_FLAG :0);
	f.can_dlc = len;

	for(int i=0;i<len;i++){
		f.data[i]=sendBuf[i];
	}

	if(::write(context->s,&f,sizeof(f)) <= 0){
		return -1;

	}

	return 1;


}



int HAL_CAN::read(char* recBuf, uint32_t* canID, bool* isExtID,bool* rtr){
	can_frame msg;

	bool _extID,_rtr;

	if(context->RxFifo.get(msg)){

		_extID = (msg.can_id & CAN_EFF_FLAG) >0;
		_rtr   = (msg.can_id & CAN_RTR_FLAG) >0;


		if(canID){
			*canID = _extID ? (msg.can_id & 0x1FFFFFFF ): (msg.can_id & 0x7FF);
		}
		if(isExtID){
			*isExtID = _extID;
		}
		if(rtr){
			*rtr = _rtr;
		}

		for(int i=0;i<8;i++){
			recBuf[i]=msg.data[i];
		}

		return msg.can_dlc;
	}

	context->rxFifoEmpty=true;
	return -1;

}


void can_sig_io_handler(int signo) {
	//PRINTF("UART IRQ\n");
	can_frame f;
	bool dataReady;

	for(int i=0;i< numHalCanInstances;i++){
		if(activeDevices[i] && activeDevices[i]->s >0){
			dataReady=false;
			while( ::read(activeDevices[i]->s,&f,sizeof(f))>0){
				//printf("red something\n");
				activeDevices[i]->RxFifo.put(f);
				activeDevices[i]->rxFifoEmpty=false;
				dataReady=true;
			}
			if(dataReady){
				activeDevices[i]->hal_can->upCallDataReady();
			}

			if(activeDevices[i]->hal_can->isWriteFinished()){
				activeDevices[i]->hal_can->upCallWriteFinished();
			}
		}

	}

}


#ifndef NO_RODOS_NAMESPACE
}
#endif
