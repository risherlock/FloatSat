
#include "rodos.h"

static Application  receiverName("UDPReciver with resumer");

static UDPIn in(5001);

class Receiver : public StaticThread<> {

    static const uint16_t DATALEN = 200;
    char userData[DATALEN];

    void run () {
	uint32_t inAddr;
	int ip0, ip1, ip2, ip3;   // Octets of sender address
	char sendHostAddress[50];  // Output address

	TIME_LOOP(3*SECONDS, 5*SECONDS) {
	    while(int len = in.get(userData, DATALEN, &inAddr)) {
		// Convert output address to IPv4 dotted decimal notation
		ip3 = inAddr & 0xff; 	inAddr >>= 8;
		ip2 = inAddr & 0xff; 	inAddr >>= 8;
		ip1 = inAddr & 0xff; 	inAddr >>= 8;
		ip0 = inAddr & 0xff; 	inAddr >>= 8;
		xsprintf(sendHostAddress, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
			    
		PRINTF("UDP Receiver: %d, '%s'; from host %s\n", len, userData, sendHostAddress);
	    }
	}
    }
};

// Instantiate the receiver
Receiver receiver;

/******************************/

class Resumer : public TimeEvent {
protected:
    void handle(){
	double t0, t1;
	t0 = SECONDS_NOW();

	if(in.readyToGet()) {
	    t1 = SECONDS_NOW();
	    PRINTF("Resuming, timedif = %f\n", t1 - t0);
	    // Wake receiver.
	    receiver.resume();
	}
	t1 = SECONDS_NOW();
    }

    void init() {
	activatePeriodic(1*SECONDS, 10*MILLISECONDS);
    }
};

// Instantiate resumer
Resumer resumer;

