/**
 * @file udp.h
 * @date 2008/05/20 6:20
 * @author Sergio Montenegro
 *
 *
 * UDP interface
 *
 */
#include "hal/udp.h"
#include "rodos-assert.h"
#include "rodos-debug.h"
#include "topic.h"



namespace RODOS {

UDPIn::UDPIn(const int32_t portNr) : udpRx(portNr) 			      { }
UDPIn::UDPIn(const int32_t portNr, Topic<GenericMsgRef>* associatedTopic) : udpRx(portNr)  {udpRx.setAsync(associatedTopic); }

void UDPIn::setAsync(Topic<GenericMsgRef>* associatedTopic)             {udpRx.setAsync(associatedTopic); }

void UDPIn::reopen(const int32_t portNr) 					{ udpRx.reopen(portNr); }

int32_t UDPIn::get(void* userData, uint16_t maxLen) 				{ return udpRx.get(userData, maxLen); }
int32_t UDPIn::get(void* userData, uint16_t maxLen, uint32_t *ipaddr) 	{ return udpRx.get(userData, maxLen,ipaddr); }
bool UDPIn::readyToGet() 						{ return udpRx.readyToGet(); }

bool UDPIn::isConnected() 						{ return udpRx.isInitialised(); }

#ifdef MULTICAST
// to use this method please define the preprocessor symbol (-D) MULTICAST in your compiler
void UDPIn::joinMulticastGroup(unsigned long ipaddr) { udpRx.joinMulticastGroup(ipaddr);}
#endif

/****************/

UDPOut::UDPOut(const int32_t portNr, const char* hostname) 		: udpTx(portNr, hostname) 	{ }
//UDPOut::UDPOut(const long _portNr) 					: udpTx(_portNr) 	  	{ }
UDPOut::UDPOut(const int32_t _portNr, uint32_t _ipAddr) 		: udpTx(_portNr, _ipAddr)	{ }
UDPOut::UDPOut(const int32_t _portNr, int ip0, int ip1, int ip2, int ip3)	: udpTx(_portNr, ip0, ip1, ip2, ip3) { }

bool UDPOut::send(const void* userData, const uint16_t maxLen) 	        	 { return udpTx.send(userData, maxLen); }
bool UDPOut::sendTo(const void* userData, const uint16_t maxLen, uint32_t ipAddr) { return udpTx.sendTo(userData, maxLen,ipAddr); }
bool UDPOut::isConnected() 						{ return udpTx.isInitialised(); }


UDPInOut::UDPInOut(int32_t broadcastPort):
    udpIn(broadcastPort),
    udpOut(broadcastPort) {
    isBroadcastLink=true;
    RODOS_ASSERT(broadcastPort < 0); // Broadcast UDPInOut defined but port number ist positive
}

UDPInOut::UDPInOut(int32_t port ,const char* hostname):
    udpIn(port),
    udpOut(port,hostname) {
    isBroadcastLink=false;
    if(port <=0) {
        RODOS_ERROR("Non-Broadcast UDPInOut defined but port number ist negative");
    }
}

UDPInOut::UDPInOut(int32_t localInputPort, int32_t remoteOutputPort,const  char* hostname):
    udpIn(localInputPort),
    udpOut(remoteOutputPort,hostname) {
    isBroadcastLink=false;
    if(localInputPort <=0 || remoteOutputPort <=0) {
        RODOS_ERROR("Non-Broadcast UDPInOut defined but port number ist negative");
    }
}


}

