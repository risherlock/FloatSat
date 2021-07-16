


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

#include "gateway/networkmessage.h"


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

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



    virtual bool putGeneric(const long topicId, const unsigned int len, const void* msg, const NetMsgInfo& netMsgInfo) {
       //ERROR("For sure, You are overriding a deprecated old put function from Putter. please override only the new (this!) putGeneric");
       return false;
    }

};


#ifndef NO_RODOS_NAMESPACE
}
#endif

