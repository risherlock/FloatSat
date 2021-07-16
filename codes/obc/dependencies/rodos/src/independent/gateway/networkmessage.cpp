/**
* @file networkMessages.cpp
* @date 2014/09/30
* @author Sergio Montenegro
*
* @brief Messages to be exchanged between nodes and network
*
*/
#include <stdint.h>


#include "gateway/networkmessage.h"
#include "misc-rodos-funcs.h"

namespace RODOS {

uint16_t NetworkMessage::setUserData(const void* data, uint16_t len) {
    len = min(len, static_cast<uint16_t>(MAX_NETWORK_MESSAGE_LENGTH));
    memcpy(userDataC, data, len);
    put_len(len);
    return len;
}

/** Copies data from message received to a user provided buffer.
 *
 * @param destination pointer to output data stream received
 * @param length of message buffer, supposed to be <= MAX_NETWORK_MESSAGE_LENGTH
 * @return length of message copied
 */
uint16_t NetworkMessage::getUserData(void* destination, uint16_t maxLen) {
    maxLen = min(maxLen, get_len());
    memcpy(destination, userDataC, maxLen);
    return maxLen;
}

} // namespace RODOS
