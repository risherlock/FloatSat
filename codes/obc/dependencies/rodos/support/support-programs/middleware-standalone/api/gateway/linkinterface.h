



#pragma once

#include "gateway/networkmessage.h"


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

/**
 * @file linkinterface.h
 * @date 2009/05/01 7:07
 * @author Sergio Montenegro
 *
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

    static int linkidentifierCounter;

protected:

    /// Gateway which has to be connected to the network / interface.
    const long linkIdentifier;


public:
    bool isBroadcastLink;

    /**
     * @brief	Constructor
     * @param	linkId	Unique identifier for the link within this node. 0 is
     * 					reserved for local broadcast messages, any other value
     * 					will do. Used to determine from which link a message
     * 					was received.
     */
    Linkinterface(long linkId) : linkIdentifier((linkId<0) ? linkidentifierCounter++ : linkId) {
        isBroadcastLink = false;
    }


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
    virtual bool getNetworkMsg(NetworkMessage &inMsg,int32_t &numberOfReceivedBytes)   { return false; }

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
    virtual bool sendNetworkMsg(NetworkMessage& outgoingMessage) { return true;  }
    inline long getLinkdentifier() const                { return this->linkIdentifier; }

    /**
     * This Thread is resumed when there is new Data availible and getNetworkMsg should be called
     *  or when buffered Messages have been transmittet on the wire.
     */

};


#ifndef NO_RODOS_NAMESPACE
}
#endif

