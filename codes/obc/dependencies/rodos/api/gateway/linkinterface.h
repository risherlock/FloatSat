
#pragma once

#include "gateway/networkmessage.h"
#include "thread.h"

namespace RODOS {

/**
 * @file linkinterface.h
 * @date 2009/05/01 7:07
 * @author Sergio Montenegro
 *
 * Copyright 2009 DLR
 *
 * @brief subscribers as gateway
 *
 */

/**
 * Interface class providing methods to connect to networks or hardware
 * interfaces and to enable data transfer.
 * The implementation is network dependent and MUST be done in derived
 * classes specific to the platform.
 */
class Linkinterface  {

    static uint32_t linkidentifierCounter;

protected:

    /// Gateway which has to be connected to the network / interface.
    const uint32_t linkIdentifier;
    Thread* threadToResume;

public:
    bool isBroadcastLink;

    /**
     * @brief	Constructor
     * @param	linkId	Unique identifier for the link within this node. 0 is
     * 					reserved for local broadcast messages, any other value
     * 					will do. Used to determine from which link a message
     * 					was received.
     */
    Linkinterface(int64_t linkId) : linkIdentifier((linkId<0) ? linkidentifierCounter++ : static_cast<uint32_t>(linkId & 0xFFFFFFFF)) {
        isBroadcastLink = false;
        threadToResume=0;
    }
    virtual ~Linkinterface() { }

    /** To initialize the hardware and to establish physical interfaces.
     * Depending on the type of interface set interrupt handlers (UART) or
     * select a port (UDP) and init the local node number if possible.
     */
    virtual void init() {}


    /**
     *Processs all received Data. When a NetworkMessage is complete it returns true
     *@param inMsg NetworkMessage to write to. When return false this may contain incomplete Data, so do not touch it
     *@param numberOfReceivedBytes When return=true this contains the number of Bytes really received or -1 if it is unknown.
     *		Should be ==Networkmessage.numberOfBytesToSend() in case of no error.
     */
    virtual bool getNetworkMsg([[gnu::unused]] NetworkMessage &inMsg, [[gnu::unused]] int32_t &numberOfReceivedBytes)   { return false; }

    /**
     * Returns if all buffered Messages have been transmittet on the wire.
     * If this returns true the next call to sendNetworkMsg should immetiatly start sending out the new messages.
     *
     */
    virtual bool isNetworkMsgSent()                     { return true; }

    /**
     *  Sends NetworkMessage over the Link. May block. When it returns the Message should be send out or have been buffered.
     * @param The Message to send
     */
    virtual bool sendNetworkMsg([[gnu::unused]] NetworkMessage& outgoingMessage) { return true;  }
    inline uint32_t getLinkdentifier() const                { return this->linkIdentifier; }

    /**
     * This Thread is resumed when there is new Data availible and getNetworkMsg should be called
     *  or when buffered Messages have been transmittet on the wire.
     */

    virtual void suspendUntilDataReady(int64_t reactivationTime = END_OF_TIME) =0 ;
};


}  // namespace


