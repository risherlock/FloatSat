

/**
* @file hw_udp.cc
* @date 2012/06/09
* @author Sergio Montenegro
*
*
* @brief UDP + UDP Braodcast communication via sockets (Linux)
*/

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

/**
*  UDP interface for Linux platform, using sockets
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> // for close()
#include <netdb.h>
// #include <netinet/in.h> // for  inet_addr
#include <arpa/inet.h> // for  inet_addr
// #include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include "rodos.h"
#include "hw_udp.h"
#include "hal/hal_uart.h"

#ifndef IP_BROADCAST_ADR
#define IP_BROADCAST_ADR "255.255.255.255"
#endif

namespace RODOS {

/*********************************************************
 ** SORRY!!!:
 ** I was not able to find a solution how to define a signal server for
 ** each udp incoming port, or how to pass a parameter to the signal server
 ** to say which udp port/soket generated the interrupt...
 **
 ** the only solution I was able to provide:
 **   store in an array all asyncrhonous upd port descriptor and
 **   for each signal from udp, search all descriptor...
 **************************************************************/

#define MAX_UDP_PORTS 20
static int                   numOfAsyncInputSockets = 0;
static int                   asyncInputSocketDescriptor[MAX_UDP_PORTS];
static Topic<GenericMsgRef>* asyncInputSocketAssociatedTopic[MAX_UDP_PORTS];
static char                  inputBuf[1400]; // as long as a UDP packet can be

void udpReader(int a);
void udpReader(int) {

    GenericMsgRef msgRef;
    msgRef.msgPtr = inputBuf;

    for(int socketCnt = 0; socketCnt < numOfAsyncInputSockets; socketCnt++) {
        while((msgRef.msgLen = static_cast<int32_t>(read(asyncInputSocketDescriptor[socketCnt], inputBuf, 1400))) > 0) {
            asyncInputSocketAssociatedTopic[socketCnt]->publishFromInterrupt(&msgRef, sizeof(GenericMsgRef));
        }
    }
}


UDPReceiver::UDPReceiver(const int port) {
    reopen(port);
}


void UDPReceiver::joinMulticastGroup(uint32_t ipaddr) {
    static struct ip_mreq command;
    //    int loop = 1;
    //    if (setsockopt ( sock,
    //                    IPPROTO_IP,
    //                    IP_MULTICAST_LOOP,
    //                    &loop, sizeof (loop)) < 0) {
    //        xprintf ("setsockopt:IP_MULTICAST_LOOP");
    //        return;
    //    }
    /* Join the broadcast group: */
    command.imr_multiaddr.s_addr = htonl(ipaddr);
    command.imr_interface.s_addr = htonl(INADDR_ANY);
    if((int)command.imr_multiaddr.s_addr == -1) {
        xprintf("224.0.0.1 is no multicast address\n");
        return;
    }
    if(setsockopt(sock,
                  IPPROTO_IP,
                  IP_ADD_MEMBERSHIP,
                  &command, sizeof(command)) < 0) {
        xprintf("setsockopt:IP_ADD_MEMBERSHIP");
    }
}


void UDPReceiver::reopen(const int port) {
    enableMultiReader = false;
    initialised       = false;
    int myPort        = port;
    if(port < 0) { myPort = -port; enableMultiReader = true; }
    if(port > UINT16_MAX) {
        xprintf("KK Reopen0 UDP-in: invalid port\n");
        return;
    }

    /* create new udp socket */
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        xprintf("!! Reopen1 UDP-in: cannot open socket port %d\n", port);
        return;
    }

    // option to allow more than one process listen to this port
    if(enableMultiReader) {
        const int on = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    }

    // fills my socket address info
    memset(&inputAddr, '\0', sizeof(inputAddr));
    inputAddr.sin_family      = AF_INET;       // IPv4
    inputAddr.sin_addr.s_addr = INADDR_ANY;    // Use no fixed IP-address for data source
    inputAddr.sin_port        = htons(static_cast<uint16_t>(myPort)); // Save portaddresss in networkorder
    int retval                = bind(sock, (const sockaddr*)&inputAddr, sizeof(inputAddr));
    if(retval != 0) {
        xprintf("!! Reopen2 UDP-in: cannot bind socket port %d\n", port);
        return;
    }

    initialised = true;
}


/******************************/


void UDPReceiver::setAsync(Topic<GenericMsgRef>* associatedTopic) {

    if(sock < 0) return;
    init_sigio_handler();

    asyncInputSocketDescriptor[numOfAsyncInputSockets]      = sock;
    asyncInputSocketAssociatedTopic[numOfAsyncInputSockets] = associatedTopic;
    if(numOfAsyncInputSockets < MAX_UDP_PORTS) numOfAsyncInputSockets++;

    fcntl(sock, F_SETFL, O_NONBLOCK);
    fcntl(sock, F_SETOWN, getpid());
    /* turn on async mode -- this is the important part which enables signal delivery */
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_ASYNC);
}


UDPReceiver::~UDPReceiver() { close(sock); }

int32_t UDPReceiver::get(void* userData, const size_t maxLen) {
    if(!initialised) return 0;
    // Non-blocking read from socket.
    ssize_t errcode = recvfrom(sock, userData, maxLen, MSG_DONTWAIT, 0, 0);
    if(errcode <= 0) {
        return 0;
    }
    return static_cast<int32_t>(errcode);
}

int32_t UDPReceiver::get(void* userData, const size_t maxLen, uint32_t* ipaddr) {
    sockaddr_in sendAddr;

    // Reset sender address information
    memset(&sendAddr, '\0', sizeof(sendAddr));
    sendAddr.sin_family = AF_INET;  // IPv4
    sendAddr.sin_port   = htons(0); // Receive from any port
    int sendAddrLen     = sizeof(sendAddr);

    ssize_t errcode = recvfrom(sock, userData, maxLen, MSG_DONTWAIT, (sockaddr*)&sendAddr, (socklen_t*)&sendAddrLen);

    if(errcode > 0) {
        *ipaddr = ntohl(sendAddr.sin_addr.s_addr);
    }

    if(errcode <= 0) {
        return 0;
    }
    return static_cast<int32_t>(errcode);
}

bool UDPReceiver::readyToGet() {
    if(!initialised) return false;
    char tempBuf[16];
    ssize_t errcode = recvfrom(sock, tempBuf, 16, MSG_PEEK | MSG_DONTWAIT, 0, 0);
    if(errcode <= 0) return false;
    return true;
}


/******************************************************************************/

void UDPTransmitter::openConnection(int32_t port, const char* host) {
    initialised = false;

    if(port < 0) {
        port            = -port;
        enableBroadCast = true;
    } else {
        enableBroadCast = false;
    }
    if(port > UINT16_MAX) {
        xprintf("!! Openconnection0 UDP-out: invalid port\n");
        return;
    }

    /** Open socket
     * PF_INET - IPv4 protocol family
     * SOCK_DGRAM - UDP, connectionlessq, unreliable datagrams
     */
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        xprintf("!! Openconnection1 UDP-out: cannot open socket\n");
        return;
    }

    // fills my socket address info
    memset(&outputAddr, '\0', sizeof(outputAddr));
    hp = gethostbyname(host);
    if(hp != 0) {
        memcpy(&outputAddr.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
    } else {
        xprintf("!! Openconnection2 UPD-out: gethostbyname failed, taking max\n");
        outputAddr.sin_addr.s_addr = inet_addr("127.255.255.255");
    }

    outputAddr.sin_family = AF_INET;
    outputAddr.sin_port   = htons(static_cast<uint16_t>(port));

    if(enableBroadCast) {
        // option to allow broadcast
        //eg 192.168.1.255. All computer in  network 192.168.1.xxx receive it.
        //   127.255.255.255 broadcast of loopback: only to local host
        //outputAddr.sin_addr.s_addr = inet_addr("127.255.255.255");
        outputAddr.sin_addr.s_addr = inet_addr(IP_BROADCAST_ADR);
        const int on               = 1;
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    }
    initialised = true;
}


UDPTransmitter::UDPTransmitter(const int32_t port, const char* host) {
    enableBroadCast = false;
    openConnection(port, host);
}


UDPTransmitter::UDPTransmitter(const int32_t portNr, uint32_t ipAddr) {
    char hostname[100];
    int  ip0, ip1, ip2, ip3;
    ip3 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip2 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip1 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip0 = ipAddr & 0xff;
    ipAddr >>= 8;
    enableBroadCast = false;

    xsprintf(hostname, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
    openConnection(portNr, hostname);
}


UDPTransmitter::UDPTransmitter(const int32_t portNr, int ip0, int ip1, int ip2, int ip3) {
    char hostname[100];
    xsprintf(hostname, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
    enableBroadCast = false;
    openConnection(portNr, hostname);
}

UDPTransmitter::~UDPTransmitter() { close(sock); }


/*************************************************/
bool UDPTransmitter::send(const void* msg, const size_t len) {
    ssize_t retval;
    if(!initialised) return false;
    retval = sendto(sock, msg, len, 0, (sockaddr*)&outputAddr, sizeof(outputAddr));
    if(retval == -1) {
        return false;
    }
    return true;
}

bool UDPTransmitter::sendTo(const void* userData, const size_t maxLen, uint32_t ipAddr) {

    // Here we establish a connection procedure like in send, but connecting to another host.
    hostent*    newHp;
    sockaddr_in newOutputAddr;

    int  ip0, ip1, ip2, ip3; // Octets of output address
    char remHostAddress[50]; // Output address

    // Convert output address to IPv4 dotted decimal notation
    ip3 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip2 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip1 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip0 = ipAddr & 0xff;
    ipAddr >>= 8;
    xsprintf(remHostAddress, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);

    // Copy host address into structure hp
    // be aware gethostbyname deprecated by POSIX
    newHp = gethostbyname(remHostAddress);

    // Fill in remote host address structure
    memset(&newOutputAddr, '\0', sizeof(newOutputAddr));
    memcpy(&newOutputAddr.sin_addr, newHp->h_addr, static_cast<size_t>(newHp->h_length));
    newOutputAddr.sin_family = AF_INET;
    newOutputAddr.sin_port   = outputAddr.sin_port;

    // Send to a different host, but the same socket.
    ssize_t sendval = sendto(sock, userData, maxLen, 0, (sockaddr*)&newOutputAddr, sizeof(newOutputAddr));
    if(sendval == -1) {
        return false;
    }
    return true;
}

void sigio_handler(int a);
void uart_sig_io_handler(int signo);
void can_sig_io_handler(int signo);

void sigio_handler(int a) {
    udpReader(a);
    uart_sig_io_handler(a);
#ifdef ENABLE_LINUX_CAN_INTERRUPT
    can_sig_io_handler(a);
#endif
}


void init_sigio_handler() { /* Warning: it was commented out in posix, I do not know why **/
    static int initialized = 0;

    if(!initialized) {
        signal(SIGIO, sigio_handler);
        initialized = 1;
    }
}


} // namespace RODOS
