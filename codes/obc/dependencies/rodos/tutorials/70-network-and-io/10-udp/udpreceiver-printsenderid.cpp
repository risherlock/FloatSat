
#include "rodos.h"

static Application  receiverName("UDPReceiver");


// Assign a port ID and bind it to a socket.
// static UDPIn in(5021);

/** 
 * A simple UDP receiver, listens to the port by polling in short time intervalls.
 */
class Receiver : public StaticThread<> {
    
    static const uint16_t BUFFSIZE = 200;
    char userData[BUFFSIZE];      // input data buffer
    uint32_t inAddr;     // sender address IPv4 
    int ip0, ip1, ip2, ip3;   // octets of sender address
    char sendHostAddress[50];  // Sender address
    
public:
    UDPIn in; // UDP port fo the receiver

    Receiver(const int32_t portNum) : in(portNum) { }

    void run () {
	TIME_LOOP(1*SECONDS, 20*MILLISECONDS) {
	    while(int len = in.get(userData, BUFFSIZE, &inAddr)) {
		// Convert sender address to IPv4 dotted decimal notation
		ip3 = inAddr & 0xff; 	inAddr >>= 8;
		ip2 = inAddr & 0xff; 	inAddr >>= 8;
		ip1 = inAddr & 0xff; 	inAddr >>= 8;
		ip0 = inAddr & 0xff; 	inAddr >>= 8;
		xsprintf(sendHostAddress, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
			    
		PRINTF("UDP Receiver: %d, %s; from host %s\n", len, userData, sendHostAddress);
	    }
	}
    }
};

/******************************/
// Instantiate receiver
static Receiver receiver(5001);

