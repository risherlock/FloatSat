
#pragma once

/**
 *  UDP Interface for posix (linux) platfrom, using Sokets
 */


#include <netdb.h>

#ifndef __APPLE__
#define IP_BROADCAST_ADR        "127.255.255.255"
#else
#define IP_BROADCAST_ADR        "224.0.0.1"
#endif



/**
 *	UDP imput port for posix (linux)
 */

class UDPInLx {
    int inputSocket;
    sockaddr_in inputAddr;
    bool isInitialized;
public:
    UDPInLx(int portNr); // nevative ports indicate boradcast
    bool get(void* userData, int maxLen=1300);
    bool isConnected() { return isInitialized; }
};


/**
 *	Output UDP ports for posix (Linux)
 */

class UDPOutLx {
    hostent *hp;
    int outputSocket;
    sockaddr_in outputAddr;
public:
    /// init UDP communication, can be re-called at any time
    UDPOutLx(long receiverPort, char *receiverHost = (char*)"localhost"); // negative port-values for boradcast
    // Send it as datagrm contining "userdata", dafault length = all bytes
    bool send(void* data, int len=1300);
};



