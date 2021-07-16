
#include "UdpHW_common.h"

#include <arpa/inet.h> // for  inet_addr

/*************************************************/

UDPInLx::UDPInLx(int port) {

        bool enableMultiReader = false;
        if(port < 0) { port = -port; enableMultiReader = true; }

	/* Create socket from which to receive. */
	inputSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (inputSocket < 0)  return;

       // option to allow more than one process listen to this port (eg. broadcast)
       if(enableMultiReader) {
          const int on = 1;
          setsockopt(inputSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
        }

	/* Create name with wildcards. */
	inputAddr.sin_family = AF_INET;
	inputAddr.sin_addr.s_addr = INADDR_ANY;
	inputAddr.sin_port =  htons(port);   // 0 to get any port
	bind(inputSocket, (sockaddr*)&inputAddr, sizeof(inputAddr));

	/* Find assigned port value and print it out. */
	socklen_t len =  sizeof(inputAddr);
	getsockname(inputSocket, (sockaddr*)&inputAddr, &len);

	/** Set to nonblocking: no wait if no data is ready */
	fcntl(inputSocket,F_SETFL, O_NONBLOCK);
}


/***********************************************/

bool UDPInLx::get(void* userData, int maxLen) {
	int errcode =  read(inputSocket, userData, maxLen);
	if(errcode <= 0) return false;
	return true;
}



/***********************************************************************************************/


UDPOutLx::UDPOutLx(int port, const char *receiverHost) {

        bool enableBroadCast = false;
        if(port < 0) { port = -port;  enableBroadCast = true; }

	outputSocket= socket(AF_INET, SOCK_DGRAM, 0);
	hp = gethostbyname(receiverHost);

	memcpy(&outputAddr.sin_addr, hp->h_addr, hp->h_length);
	outputAddr.sin_family = AF_INET;
	outputAddr.sin_port = htons(port);

        if(enableBroadCast) {
            //eg 192.168.1.255. All computer in  network 192.168.1.xxx receive it.
            //   127.255.255.255 broadcast of loopback: only to local host
            outputAddr.sin_addr.s_addr = inet_addr("127.255.255.255");
            const int on = 1;
            setsockopt(outputSocket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on) );
         }
}


/*************************************************/


bool UDPOutLx::send(void* data, int len) {

	sendto(outputSocket, data, len, 0, (sockaddr*)&outputAddr, sizeof(outputAddr));
	return true;
}


/**********************************************/






