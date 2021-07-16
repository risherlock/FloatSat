
#pragma once

/**
 *  UDP Interface for posix (linux) platfrom, using Sokets
 */

#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/**
 *	UDP imput port for posix (linux)
 */

class UDPInLx {
	int inputSocket;
	sockaddr_in inputAddr;
public:
	UDPInLx(int portNr); // nevative ports indicate boradcast
	bool get(void* userData, int maxLen=1300);
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
	UDPOutLx(int receiverPort, const char *receiverHost); // negative port-values for boradcast
	// Send it as datagrm contining "userdata", dafault length = all bytes
	bool send(void* data, int len=1300);
};



