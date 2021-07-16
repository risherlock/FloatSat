
#pragma once

#include "hw_udp.h"
#include "topic.h"

/**
 * @file udp.h
 * @date 2008/05/20 6:20
 * @author Sergio Montenegro
 *
 *
 * UDP interface
 *
 */



//eg 192.168.1.255. All computer in  network 192.168.1.xxx receive it. (your home network)
//   127.255.255.255 broadcast of loopback: only to local host
//   255.255.255.255 The whole local network
//   127.0.0.1       only my local host

//constexpr char IP_BROADCAST_ADR[] = "192.168.1.255";
//constexpr char IP_BROADCAST_ADR[] = "255.255.255.255";

#ifndef __APPLE__
constexpr char IP_BROADCAST_ADR[] = "127.255.255.255";
#else
constexpr char IP_BROADCAST_ADR[] = "224.0.0.1";
#endif


namespace RODOS {

///To receive data from a single UDP port (socket) - abstraction layer.
class UDPIn {
    /// socket API, used internally for the HW Implementation
    UDPReceiver udpRx;
public:
    /** Constructor, opens an UDP connection to the given port
     * @param[in] portNr port number on localhost used for reception
     */
    UDPIn(const int32_t portNr);


    /** Constructor, opens an UDP connection to the given port
     * any incommig messages will be distributed under the given topic
     * The get method is in the case not accessible!
     * Warning: The distribution is from Interrupt server, please use
     * Subscriber::putFromInterrupt(...)
     * do not suspend (no semaphore, no yield, etc) in this method!
     */
    UDPIn(const int32_t portNr, Topic<GenericMsgRef>* associatedTopic);

    void setAsync(Topic<GenericMsgRef>* associatedTopic);

    /** Opens a socket for UDP reception.
     * On success attribute 'initialised' is set true.
     * @param[in] portNr port number on localhost
     */
    void reopen(const int32_t portNr);

    /**
     * Receives data from UDP socket. Writes data up to maxLen to userData.
     * @param[in,out] userData pointer to input buffer
     * @param[in] maxLen size of input buffer
     * @return length of message written to userData
     */
    int32_t get(void* userData, uint16_t maxLen);

    /**
     * Receives data from UDP socket. Writes data up to maxLen to userData.
     * @param[in,out] userData pointer to input buffer
     * @param[in] maxLen size of input buffer
     * @param[out] ipaddr sender address
     * @return length of message written to userData
     */
    int32_t get(void* userData, uint16_t maxLen, uint32_t *ipaddr);

    /**
     * Check for incoming data.
     * @return true - if data is available, false - otherwise
     */
    bool readyToGet();

    /**
     * Check whether port is active or not.
     * @return true - connected, false - not connected
     */
    bool isConnected();

#ifdef MULTICAST
// to use this method please define the preprocessor symbol (-D) MULTICAST in your compiler
    void joinMulticastGroup(uint32_t ipaddr);
#endif
};




///To send data to a single UPD port (socket) - abstraction layer.
class UDPOut {
    /// socket API, used internally for the HW Implementation
    UDPTransmitter udpTx;
public:

    /** Constructor for connection to remote host known by name.
     * WARNING: Negativ port number uses the abs port number and sends broadcast
     * WARNING: Name resolution not yet supported on bare-metal !
     * @param portNr port number on remote host, negativ -> broadcast to this port
     * @param hostname IP-hostname of remote host
     */
    UDPOut(const int32_t portNr, const char* hostname = "localhost");

    /** Constructor for connection to remote host known by IP-address.
     * @param _portNr port number on remote host
     * @param _ipAddr IP-address of remote host, e.g. format as hex 0x01020304,
     * where 01 = ip_part1, ...
     */
    UDPOut(const int32_t _portNr, uint32_t _ipAddr); //MSul

    /** Constructor for connection to remote host known by IP-address.
     * @param _portNr port number on remote host
     * @param ip0, ip1, ip2, ip3 IP-address of remote host consisting of separate octets
     */
    UDPOut(const int32_t _portNr, int ip0, int ip1, int ip2, int ip3);//MSul

    /** to change destination, port and host for followin sends */
    inline void reopen(const int32_t port,  const char *host = "localhost") {
        udpTx.openConnection(port,  host); 
    }

    /** Send user data
     * @param[in] userData pointer to output data stream
     * @param[in] maxLen size of data block to be send, must be <= max allowed length
     * @return true - if sending was successful, false - otherwise
     */
    bool send(const void* userData, const uint16_t maxLen);

    /** Send user data to a given address
     * @param[in] userData pointer to output data stream
     * @param[in] maxLen size of data block to be send, must be <= max allowed length
     * @param[in] ipAddr new IP-address, where data is send to
     * @return true - if sending was successful, false - otherwise
     */
    bool sendTo(const void* userData, const uint16_t maxLen, uint32_t ipAddr);//MSul

    /**
     * Check whether port is active or not.
     * @return true - connected, false - not connected
     */
    bool isConnected();
};


class UDPInOut {
public:

    UDPIn udpIn;
    UDPOut udpOut;

    bool isBroadcastLink;

    UDPInOut(int32_t broadcastPort);
    UDPInOut(int32_t port,const char* hostname );
    UDPInOut(int32_t localInputPort, int32_t remoteOutputPort,const char* hostname);
};


}  // namespace

