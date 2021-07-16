



#include"gateway/linkinterfaceudp.h"

#include "gateway/gateway.h"


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

/**
 * @file linkinterfaceudp-broadcast.cc
 * @date 2012/09/01, 2009/05/18, 2012
 * @author Sergio Montenegro, uni Würzburg
 *
 * Link Interface to broadcast networks simulated using UDP messages.
 * incomming messages will be distributed asyncrhonously using the middleware.
 *
 */


/****************************************************/

LinkinterfaceUDP::LinkinterfaceUDP(UDPInOut* udpInOut, long identifier) : Linkinterface(identifier)
     {
    udpFromNetwork = &(udpInOut->udpIn);
    udpToNetwork = &(udpInOut->udpOut);
    isBroadcastLink = udpInOut->isBroadcastLink;
    newMessage=false;
}


void LinkinterfaceUDP::init() {
    Linkinterface::init();
    // WARNING: here we shall get a unique node number, we can not now
    // we just take the defaoult from hwInt
    udpFromNetwork->setAsync(this); // Distribute incomming messages asyncromous to topic defaultga....
}

/************* Connection to the network, network and protocol dependent ***/

bool LinkinterfaceUDP::sendNetworkMsg(NetworkMessage &outMsg)	{
    return udpToNetwork->send(&outMsg, outMsg.numberOfBytesToSend());
}

bool LinkinterfaceUDP::putGeneric(const long topicId, const unsigned int len, const void* msg, const NetMsgInfo& netMsgInfo) {
    incoming.put(*((NetworkMessage*)msg));
    return true;
}


bool LinkinterfaceUDP::getNetworkMsg(NetworkMessage &inMsg,int32_t &numberOfReceivedBytes) {
    numberOfReceivedBytes = -1;
    return incoming.get(inMsg);
}

#ifndef NO_RODOS_NAMESPACE
}
#endif
