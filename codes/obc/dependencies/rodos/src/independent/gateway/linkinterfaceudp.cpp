
#include"gateway/linkinterfaceudp.h"
#include "hw_specific.h"

#include "gateway/gateway.h"


namespace RODOS {

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

LinkinterfaceUDP::LinkinterfaceUDP(UDPInOut* udpInOut, int64_t identifier) : Linkinterface(identifier), Subscriber(udpAsyncTopic),
    udpAsyncTopic(identifier,"udp async topic") {
    udpFromNetwork = &(udpInOut->udpIn);
    udpToNetwork = &(udpInOut->udpOut);
    isBroadcastLink = udpInOut->isBroadcastLink;
    newMessage=false;
}


void LinkinterfaceUDP::init() {
    Linkinterface::init();
    // WARNING: here we shall get a unique node number, we can not now
    // we just take the defaoult from hwInt
    threadToResume= Thread::getCurrentThread();
    udpFromNetwork->setAsync(&udpAsyncTopic); // Distribute incomming messages asyncromous to topic defaultga....
}

/************* Connection to the network, network and protocol dependent ***/

bool LinkinterfaceUDP::sendNetworkMsg(NetworkMessage &outMsg)	{
    return udpToNetwork->send(&outMsg, outMsg.numberOfBytesToSend());
}

void LinkinterfaceUDP::putFromInterrupt([[gnu::unused]] const uint32_t topicId, const void* any, [[gnu::unused]] size_t len) {
    const GenericMsgRef* msg = (const GenericMsgRef*)any;
    incoming.put(*((const NetworkMessage*)msg->msgPtr));
    if(threadToResume) threadToResume->resume();
}


bool LinkinterfaceUDP::getNetworkMsg(NetworkMessage &inMsg,int32_t &numberOfReceivedBytes) {
    numberOfReceivedBytes = -1;
    return incoming.get(inMsg);
}

void LinkinterfaceUDP::suspendUntilDataReady(int64_t reactivationTime){
	//Not yet race condition safe
	Thread::suspendCallerUntil(reactivationTime);
}

}
