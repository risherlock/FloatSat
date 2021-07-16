



/**
 * @file putter.h
 * @date 2008/04/28 7:20
 * @author Sergio Montenegro, Lutz Dittrich
 *
 *
 *
 * @brief Superclass to elements which can accept/store data (header)
 *
 */

#pragma once

#include "rodos-debug.h"
#include "subscriber.h"


namespace RODOS {

/**
 * @class Putter
 * @brief Virtual superclass for classes which implement a put function
 *
 * Virtual superclass for classes which implement a put function
 * to store data, in order to have a generic interface to data storage,
 * eg. Fifo, CommBuffer, SyncFifo.
 */
class Putter {
public:

    virtual ~Putter() {};

    virtual bool putGeneric([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, [[gnu::unused]] const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
       RODOS_ERROR("If you see this message, You are overriding a deprecated old put function from Putter. please override only the new (this!) putGeneric");
       return false;
    }

};


}  // namespace

