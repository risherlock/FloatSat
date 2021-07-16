/**
* @file hw_udp.h
* @date 2008/07/15 16:35
* @author Sergio Montenegro, Lutz Dittrich
*
*
* @brief UDP communication
*/

#ifndef __HW_UDP_H__
#define __HW_UDP_H__

#include <stdint.h>
#include <sys/types.h>


#include "lwip/udp.h"
#include "hw_datatypes.h"
#include "topic.h"

#define RECV_BUFFER_COUNT 8

namespace RODOS {

class UDPReceiver {
    friend void udp_recv_func(void* arg, struct udp_pcb* pcb, struct pbuf* p, ip_addr_t* addr, u16_t port);

  private:
    udp_pcb*              pcb;
    Topic<GenericMsgRef>* asyncTopic;
    GenericMsgRef         recvDatRef[RECV_BUFFER_COUNT];
    uint8_t               recvDat[RECV_BUFFER_COUNT][MAX_NETWORK_MESSAGE_LENGTH];
    uint8_t               recvDatPtr;

    bool     multiReader;
    uint16_t listenPort;

    bool init();

  public:
    /** Constructor
     * Warning: negative port number means more than one can receive on the same
     * @param portNr port number on localhost used for reception , negative for broadcast
     */
    UDPReceiver(const TUDPPortNr port);
    /** Destructor */
    ~UDPReceiver();

    /** Opens a socket for UDP reception.
     * On success attribute 'initialised' is set true.
     * Warning: negative port number means more than one can receive on the same
     * @param portNr port number on localhost, negative for broadcast
     */
    void reopen(const TUDPPortNr port);

    void setAsync(Topic<GenericMsgRef>* associatedTopic);

#ifdef MULTICAST
    void joinMulticastGroup(unsigned long ipaddr);
#endif
    /**
     * TODO: Not yet supported on bare-metal, use asyncTopic (UDPReceiver::setAsync)
     * Receives data from interface. Writes data up to maxLen to userData.
     * @param[IN/OUT] userData pointer to input buffer
     * @param[IN] size of input buffer
     * @return length of message written to userData
     */
    int32_t get(void* userData, const uint16_t maxLen = 1300);
    int32_t get(void* userData, uint16_t maxLen, uint32_t* ipaddr); // return number of bytes read, or 0, or < 0 if error

    bool readyToGet();

    bool isInitialised() { return initialised; } // due to windows compatibility

  private:
    bool initialised;
};


/**
*	output UDP port
*/
class UDPTransmitter {
  private:
    bool      enableBroadCast;
    ip_addr_t ip;
    uint16_t  port;

    udp_pcb* pcb;
    bool     connected;
    bool     initialised;

    void init(const TUDPPortNr _port, ip_addr_t ipaddr);
    void connect(ip_addr_t _ip, uint16_t prt);

  public:
    /** init udp communication, can be re-called at any time
     ** WARNING: Negative port number means broadcast **/

    UDPTransmitter(const TUDPPortNr _port, const char* host = "localhost");
    UDPTransmitter(const long _portNr, uint32_t _ipAddr); // _ipAddr as eg. 192.168.0.20
    UDPTransmitter(const long _portNr, int ip0, int ip1, int ip2, int ip3);
    virtual ~UDPTransmitter();

    void openConnection(const TUDPPortNr port, const char* host);

    /** Send it as datagram containing "userdata", default length = all bytes
     * @param data pointer to data field
     * @param length of data field
     */
    virtual bool send(const void* msg, const uint16_t len);

    /** Send datagram to a specified host different to the address used during intialization
     * @param data pointer to data field
     * @param length of data field
     * @param ipAddr of receiving host
     */
    bool sendTo(const void* userData, const uint16_t maxLen, uint32_t ipAddr);
    bool isInitialised() { return initialised; } // due to windows compatibility
};


/********* Global Functions **************/

bool setMyIP(int ip0, int ip1, int ip2, int ip3);     // eg. setMyIP(192,168,20,10);
bool setMyIPMask(int ip0, int ip1, int ip2, int ip3); // eg. setMyIPMask(255,255,0,0);

} /* namespace RODOS */

#endif /* __HW_UDP_H__ */
