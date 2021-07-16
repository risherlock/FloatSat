



/**
* @file hw_udp.h
* @date 2008/07/15 16:35
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief UDP communication via sockets (Linux)
*/

#ifndef __HW_UDP_H__
#define __HW_UDP_H__

#include <stdint.h>
#include "putter.h"

typedef int TUDPPortNr;


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

/**
*  UDP interface for Linux platform, using sockets
*/


/**
 *    UDP input port for linux
 */
class UDPReceiver {

private:
    int sock;
    //sockaddr_in inputAddr;
    bool enableMultiReader;

public:
    /** Constructor
     * Warning: negativ port number menans more than one can receive on the same
     * @param portNr port number on localhost used for reception , negativ for boradcast
     */
    UDPReceiver(const TUDPPortNr port);
    /** Destructor */
    ~UDPReceiver();

    /** Opens a socket for UDP reception.
     * On success attribute 'initialised' is set true.
     * Warning: negativ port number menans more than one can receive on the same
     * @param portNr port number on localhost, negative for broadcast
     */
    void reopen(const TUDPPortNr port);
 
    void setAsync(Putter* associatedTopic);

    /**
     * Receives data from Linux UDP socket. Writes data up to maxLen to userData.
     * @param[IN/OUT] userData pointer to input buffer
     * @param[IN] size of input buffer
     * @return length of message written to userData
     */
    long get(void* userData, const unsigned int maxLen=1300);
    long get(void* userData, int maxLen, unsigned long *ipaddr); // return number of bytes read, or 0, or < 0 if error
    bool readyToGet();
  bool isInitialised()    { return initialised; } // due to windows compatibility
private:
    bool initialised;
};


/**
*	output UDP port for linux
*/
class UDPTransmitter {
private:
    int sock;
    struct hostent* hp;
   // sockaddr_in outputAddr;

    bool enableBroadCast;

    void openConnection(const TUDPPortNr port,  const char *host);

public:

    /** init udp communication, can be re-called at any time
     ** WARNING: Negative port number means broadcast **/

    UDPTransmitter(const TUDPPortNr port,  const char *host = "localhost");
    UDPTransmitter(const long _portNr, unsigned long _ipAddr); // _ipAddr as eg. 192.168.0.20
    UDPTransmitter(const long _portNr, int ip0, int ip1, int ip2, int ip3);
    virtual ~UDPTransmitter();

    /** Send it as datagram contining "userdata", default length = all bytes
     * @param data pointer to datafield
     * @param length of data field
     */
    virtual bool send(const void*msg, const unsigned int len);

    /** Send datagram to a specified host different to the address used during intialization
     * @param data pointer to datafield
     * @param length of data field
     * @param ipAddr of receiving host
     */
    bool sendTo(const void* userData, const int maxLen, unsigned long ipAddr);
  bool isInitialised()    { return initialised; } // due to windows compatibility
private:
    bool initialised;
};



/********* Global Fuctions **************/

bool setMyIP(int ip0, int ip1, int ip2, int ip3); // eg. setMyIP(192,168,20,10);
bool setMyIPMask(int ip0, int ip1, int ip2, int ip3); // eg. setMyIPMask(255,255,0,0);


#ifndef NO_RODOS_NAMESPACE
}
#endif

#endif /* __HW_UDP_H__ */



