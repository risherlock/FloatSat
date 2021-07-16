
/**
* @file hw_udp.h
* @date 2008/07/15 16:35
* @author Sergio Montenegro
*
*/

#include "topic.h"

#pragma once

namespace RODOS {

/**
*  UDP interface for Linux platform, using sockets
*/


/**
 *    UDP input port for linux
 */
class UDPReceiver {

private:

public:
    UDPReceiver([[gnu::unused]] const int port) { }
    ~UDPReceiver()              { }

    void reopen([[gnu::unused]] const int port) { }
 
    void setAsync([[gnu::unused]] Topic<GenericMsgRef>* associatedTopic) { }

    int32_t get([[gnu::unused]] void* userData, [[gnu::unused]] const size_t maxLen=1300)    { return 0; }
    int32_t get([[gnu::unused]] void* userData, [[gnu::unused]] size_t maxLen, [[gnu::unused]] uint32_t *ipaddr) { return 0; }
    bool readyToGet()                                           { return false;}
    bool isInitialised()                                        { return true; } 
};


class UDPTransmitter {
private:

public:

    /** init udp communication, can be re-called at any time
     ** WARNING: Negative port number means broadcast **/

    UDPTransmitter([[gnu::unused]] const int port,  [[gnu::unused]] const char *host = "localhost")         { }
    UDPTransmitter([[gnu::unused]] const long _portNr, [[gnu::unused]] unsigned long _ipAddr)               { }
    UDPTransmitter([[gnu::unused]] const long _portNr, [[gnu::unused]] int ip0, [[gnu::unused]] int ip1, [[gnu::unused]] int ip2, [[gnu::unused]] int ip3)  { }
    virtual ~UDPTransmitter()                                               { }

    void openConnection([[gnu::unused]] const int port, [[gnu::unused]] const char *host)                  { }

    /** Send it as datagram containing "userdata", default length = all bytes
     * @param data pointer to datafield
     * @param length of data field
     */
    virtual bool send([[gnu::unused]] const void*msg, [[gnu::unused]] const unsigned int len)               { return false; }

    /** Send datagram to a specified host different to the address used during initialisation
     * @param data pointer to datafield
     * @param length of data field
     * @param ipAddr of receiving host
     */
    bool sendTo([[gnu::unused]] const void* userData, [[gnu::unused]] const int maxLen, [[gnu::unused]] unsigned long ipAddr) { return false; }
    bool isInitialised()                                                      { return true; } 
};

} // namespace

