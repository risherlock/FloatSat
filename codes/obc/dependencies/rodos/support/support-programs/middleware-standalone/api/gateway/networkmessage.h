


/**
* @file networkMessages.h
* @date 2013/06/30
* @author Sergio Montenegro
*
* @brief Messages to be exchanged between nodes and network
*
*/

#pragma once

#define MAX_NETWORK_MESSAGE_LENGTH 1300

#include <algorithm>
#include <cstdint>
#include <cstring>
#include "checksumes.h"
#include "stream-bytesex.h"

/******** Generic Network Message ***************/

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

/**
 * Simple message data protocol to transmit data to a remote node.
 */
class NetworkMessage {
    static const uint16_t HEADER_SIZE = 26;
    uint8_t header [HEADER_SIZE];
public:
    inline void    put_checkSum(uint16_t x)          {uint16_tToBigEndian(header+0, x); }
    inline void    put_senderNode(int32_t x)         {int32_tToBigEndian(header+2, x); }
    inline void    put_sentTime (int64_t x)          {int64_tToBigEndian(header+6, x); }
    inline void    put_senderThreadId(uint32_t x)    {uint32_tToBigEndian(header+14, x); }
    inline void    put_topicId(uint32_t x)           {uint32_tToBigEndian(header+18, x); }
    inline void    put_maxStepsToForward(int16_t x)  {int16_tToBigEndian(header+22, x); }
    inline void    put_len(uint16_t x)               {uint16_tToBigEndian(header+24, x); }

    inline uint16_t get_checksum()             const { return bigEndianToUint16_t(header+0); }
    inline int32_t  get_senderNode()           const { return bigEndianToInt32_t(header+2); }
    inline int64_t  get_sentTime ()            const { return bigEndianToInt64_t(header+6); }
    inline uint32_t get_senderThreadId()       const { return bigEndianToUint32_t(header+14); }
    inline uint32_t get_topicId()              const { return bigEndianToUint32_t(header+18); }
    inline int16_t  get_maxStepsToForward()    const { return bigEndianToInt16_t(header+22); }
    inline uint16_t get_len()                  const { return bigEndianToUint16_t(header+24); }

    uint8_t userDataC[MAX_NETWORK_MESSAGE_LENGTH]; ///< local buffer for user data

    /**
     * Copy constructor and assign operator that only copes nesacary data.
     */
    NetworkMessage() { }
    NetworkMessage(const NetworkMessage &oldMsg) { memcpy(this, &oldMsg, oldMsg.numberOfBytesToSend()); }
    NetworkMessage& operator= (const NetworkMessage& oldMsg) {
        if(this != & oldMsg) memcpy(this, &oldMsg, oldMsg.numberOfBytesToSend());
        return *this;
    }

    inline void dec_maxStepsToForward()    { put_maxStepsToForward(static_cast<int16_t>(get_maxStepsToForward() - 1)); }

    /** Copies user generated data into the message body.
     *
     * @param data pointer to input data stream to be transfered
     * @param length of message, supposed to be <= MAX_NETWORK_MESSAGE_LENGTH
     * @return length of message written
     */
    uint16_t setUserData(const void* data, uint16_t len) {
        len =  std::min(len, static_cast<uint16_t>(MAX_NETWORK_MESSAGE_LENGTH));
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
    uint16_t getUserData(void* destination, uint16_t maxLen) {
        maxLen = std::min(maxLen, get_len());
        memcpy(destination, userDataC, maxLen);
        return maxLen;
    }

    /* WARNING: Len has to be set befor you call this.  **/
    uint16_t numberOfBytesToSend() const { return static_cast<uint16_t>(HEADER_SIZE + get_len()); }
    uint16_t calculateCheckSum()         { return checkSum(header+2, HEADER_SIZE-2u + get_len()); }
    bool     isCheckSumOk()              { return calculateCheckSum() == get_checksum(); }
    void     setCheckSum()               { put_checkSum(calculateCheckSum()); }

} __attribute__((packed));



class NetMsgInfo {
public:
    int32_t senderNode;	///< Node ID of sending instance of RODOS
    int64_t sentTime;   ///< Time in localTime units
    uint32_t senderThreadId;
    uint32_t linkId;
};


#ifndef NO_RODOS_NAMESPACE
}
#endif


