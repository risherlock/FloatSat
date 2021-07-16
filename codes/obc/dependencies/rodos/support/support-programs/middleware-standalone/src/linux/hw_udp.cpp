



/**
* @file hw_udp.cc
* @date 2012/06/09
* @author Sergio Montenegro
*
*
* @brief UDP + UDP Braodcast communication via sockets (Linux)
*/

#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/stat.h>

/**
*  UDP interface for Linux platform, using sockets
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> // for close()
#include <netdb.h>
// #include <netinet/in.h> // for  inet_addr
#include <arpa/inet.h> // for  inet_addr
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include "hw_udp.h"
//#include "hal/hal_uart.h"
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
static int numOfAsyncInputSockets = 0;
static int asyncInputSocketDescriptor[MAX_UDP_PORTS];
static Putter* asyncInputSocketAssociatedTopic[MAX_UDP_PORTS];
static char inputBuf[1400];// as long as a UDP packet can be

void udpReader(int a) {

    int len;
    NetMsgInfo dummy;


    for(int socketCnt = 0; socketCnt < numOfAsyncInputSockets; socketCnt++) {
        while((len = read(asyncInputSocketDescriptor[socketCnt], inputBuf, 1400)) > 0) {
            asyncInputSocketAssociatedTopic[socketCnt]->putGeneric(0,len,inputBuf,dummy);
        }
    }
}


UDPReceiver::UDPReceiver(const TUDPPortNr port) {
    reopen(port);
}


void UDPReceiver::reopen(const TUDPPortNr port) {
    if(sock) close(sock);

    enableMultiReader = false;
    initialised = false;
    int myPort = port;
    if(port < 0) { myPort = -port; enableMultiReader = true; }

    /* create new udp socket */
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("!! Middleware: UDP-in: cannot open socket\n");
        return;
    }

    // option to allow more than one process listen to this port
    if(enableMultiReader) {
        const int on = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    }

    // fills my socket address info
    memset(&inputAddr, '\0', sizeof(inputAddr));
    inputAddr.sin_family = AF_INET;  		// IPv4
    inputAddr.sin_addr.s_addr = INADDR_ANY;	// Use no fixed IP-address for data source
    inputAddr.sin_port = htons(myPort);		// Save portaddresss in networkorder
    int retval = bind(sock, (const sockaddr*) &inputAddr, sizeof(inputAddr));
    if (retval != 0) {
        printf("!! Middleware UDP-in: cannot bind socket\n");
        return;
    }

    initialised = true;
}


/******************************/


void UDPReceiver::setAsync(Putter* associatedTopic) {

	if(sock < 0) return;
	init_sigio_handler();

    asyncInputSocketDescriptor[numOfAsyncInputSockets]      = sock;
    asyncInputSocketAssociatedTopic[numOfAsyncInputSockets] = associatedTopic;
    if(numOfAsyncInputSockets < MAX_UDP_PORTS) numOfAsyncInputSockets++;

    fcntl(sock,F_SETFL, O_NONBLOCK);
    fcntl(sock, F_SETOWN, getpid());
    /* turn on async mode -- this is the important part which enables signal delivery */
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_ASYNC);
}



UDPReceiver::~UDPReceiver() {close(sock); }

long UDPReceiver::get(void* userData, const unsigned int maxLen) {
    if(!initialised) return 0;
    // Non-blocking read from socket.
    int errcode =  recvfrom(sock, userData, maxLen,MSG_DONTWAIT,0,0);
    if (errcode <= 0) {
        return 0;
    }
    return errcode;
}

long UDPReceiver::get(void* userData, int maxLen, unsigned long *ipaddr) {
    sockaddr_in sendAddr;

    // Reset sender address information
    memset(&sendAddr, '\0', sizeof(sendAddr) );
    sendAddr.sin_family = AF_INET;    // IPv4
    sendAddr.sin_port = htons(0);     // Receive from any port
    int sendAddrLen = sizeof(sendAddr);

    int errcode =  recvfrom(sock, userData, maxLen, MSG_DONTWAIT, (sockaddr *) &sendAddr, (socklen_t *) &sendAddrLen);

    if (errcode > 0) {
        *ipaddr = ntohl(sendAddr.sin_addr.s_addr);
    }

    if(errcode <= 0) {
        return 0;
    }
    return errcode;
}

bool UDPReceiver::readyToGet() {
    if(!initialised) return false;
    char tempBuf[16];
    int errcode =  recvfrom(sock, tempBuf, 16, MSG_PEEK | MSG_DONTWAIT,0,0);
    if (errcode <= 0) return false;
    return true;
}



/******************************************************************************/

void UDPTransmitter::openConnection(const TUDPPortNr port, const char *host) {
    initialised = false;
    /** Open socket
     * PF_INET - IPv4 protocol family
     * SOCK_DGRAM - UDP, connectionlessq, unreliable datagrams
     */
    sock=socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("!! Middleware UDP-out: cannot open socket\n");
        return;
    }

    // fills my socket address info
    memset(&outputAddr,'\0',sizeof(outputAddr));
    hp = gethostbyname(host);
    if (hp != 0) {
        memcpy(&outputAddr.sin_addr, hp->h_addr, hp->h_length);
    } else {
        printf("!! Middleware UPD-out: gethostbyname failed, taking max\n");
        outputAddr.sin_addr.s_addr = inet_addr("127.255.255.255");
    }

    outputAddr.sin_family = AF_INET;
    outputAddr.sin_port = htons(port);

    if(enableBroadCast) {
        // option to allow broadcast
        //eg 192.168.1.255. All computer in  network 192.168.1.xxx receive it.
        //   127.255.255.255 broadcast of loopback: only to local host
        //outputAddr.sin_addr.s_addr = inet_addr("127.255.255.255");
        outputAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
        const int on = 1;
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on) );
    }
    initialised = true;
}


UDPTransmitter::UDPTransmitter(const TUDPPortNr port, const char *host) {
    enableBroadCast = false;
    int myPort = port;
    if(port < 0) { myPort = -port;  enableBroadCast = true; }
    openConnection(myPort, host);
}


UDPTransmitter::UDPTransmitter(const long portNr, unsigned long ipAddr) {
    char hostname[100];
    int ip0, ip1, ip2, ip3;
    ip3 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip2 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip1 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip0 = ipAddr & 0xff;
    ipAddr >>= 8;
    enableBroadCast = false;

    sprintf(hostname, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
    int myPort = portNr;
    if(portNr < 0) { myPort = -portNr;  enableBroadCast = true; }
    openConnection(myPort, hostname);
}


UDPTransmitter::UDPTransmitter(const long portNr, int ip0, int ip1, int ip2, int ip3) {
    char hostname[100];
    sprintf(hostname, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
    enableBroadCast = false;
    int myPort = portNr;
    if(portNr < 0) { myPort = -portNr;  enableBroadCast = true; }
    openConnection(myPort, hostname);
}

UDPTransmitter::~UDPTransmitter() { close(sock);}


/*************************************************/
void UDPTransmitter::reopen(const TUDPPortNr port,  const char *host) {
    if(sock) close(sock);

    enableBroadCast = false;
    int myPort = port;
    if(port < 0) { myPort = -port;  enableBroadCast = true; }
    openConnection(myPort, host);
}

bool UDPTransmitter::send(const void* msg, const unsigned int len) {
    int retval;
    if(!initialised) return false;
    retval=sendto(sock, msg, len, 0, (sockaddr*)&outputAddr, sizeof(outputAddr));
    if (retval == -1) {
        return false;
    }
    return true;
}

bool  UDPTransmitter::sendTo(const void* userData, const int maxLen, unsigned long ipAddr) {

    // Here we establish a connection procedure like in send, but connecting to another host.
    hostent* newHp;
    sockaddr_in newOutputAddr;

    int ip0, ip1, ip2, ip3;   // Octets of output address
    char remHostAddress[50];  // Output address

    // Convert output address to IPv4 dotted decimal notation
    ip3 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip2 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip1 = ipAddr & 0xff;
    ipAddr >>= 8;
    ip0 = ipAddr & 0xff;
    ipAddr >>= 8;
    sprintf(remHostAddress, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);

    // Copy host address into structure hp
    // TODO gethostbyname depreciated by POSIX
    newHp = gethostbyname(remHostAddress);

    // Fill in remote host address structure
    memset(&newOutputAddr, '\0', sizeof(newOutputAddr) );
    memcpy(&newOutputAddr.sin_addr, newHp->h_addr, newHp->h_length);
    newOutputAddr.sin_family = AF_INET;
    newOutputAddr.sin_port = outputAddr.sin_port;

    // Send to a different host, but the same socket.
    int sendval = sendto(sock, userData, maxLen, 0, (sockaddr*) &newOutputAddr, sizeof(newOutputAddr));
    if (sendval == -1) {
        return false;
    }
    return true;

}

void uart_sig_io_handler(int signo);
void can_sig_io_handler(int signo);

void sigio_handler(int a){

	udpReader(a);
	//uart_sig_io_handler(a);
	//can_sig_io_handler(a);

}


void init_sigio_handler()
{
	static int initialized=0;

	if(!initialized){
		 signal(SIGIO, sigio_handler);
		 initialized=1;
	}
}


}
