
#include "rodos.h"


static Application senderName("UDPSender");

static UDPOut out(5001);

class Sender : public StaticThread<> {
	char outbuf[200];

	void run () {
		int  cnt = 0;
		while(1) {
			cnt++;
			PRINTF("Sending nr %d\n", cnt);
			xsprintf(outbuf,"from UDPSender %d\n", cnt);
			out.send(outbuf, 200);
			if((cnt % 15) == 0) {
				PRINTF("Sender Wainting 3 seconds\n");
				suspendCallerUntil(NOW() + 3*SECONDS);
			}
		}
	}
};

static Sender   sender;
