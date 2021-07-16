/*
 * linkinterfaceudp-broadcast.h
 *
 *  Created on: 27.03.2013
 *      Author: erik
 */

#pragma once

#include "fifo.h"
#include "gateway/linkinterface.h"
#include "hal/udp.h"
#include "subscriber.h"
#include "topic.h"


namespace RODOS {

class LinkinterfaceUDP : public Linkinterface,Subscriber  {

protected:
    /// UDP transmitter connecting via UDP to the network switch.
    UDPIn*  udpFromNetwork;
    UDPOut* udpToNetwork;

private:

    Topic<GenericMsgRef> udpAsyncTopic;
    Fifo<NetworkMessage, UDP_INCOMMIG_BUF_LEN> incoming;
    //CommBuffer<NetworkMessage> incoming;
    bool newMessage;

protected: //This is for LinkinterfaceUDP
public:

    LinkinterfaceUDP(UDPInOut* udpInOut, int64_t identifier = -1);

    virtual ~LinkinterfaceUDP() { }
    /**
     * Link interface to the gateway, look up a free UDP input port and
     * assign a corresponding node number used as ID in the network.
     */
    void init();

    /**
     * Retrieve a message in network potocol format from an associated UDP port.
     * @param outMsg pointer to message to send wrapped in network structure
     * @return true - if sending was successful, false - otherwise
     */
    bool sendNetworkMsg(NetworkMessage &outMsg);
    bool getNetworkMsg(NetworkMessage &inMsg,int32_t &numberOfReceivedBytes);

    virtual void putFromInterrupt(const uint32_t topicId, const void* any, size_t len = 0);
    virtual void suspendUntilDataReady(int64_t reactivationTime = END_OF_TIME);
};

}

