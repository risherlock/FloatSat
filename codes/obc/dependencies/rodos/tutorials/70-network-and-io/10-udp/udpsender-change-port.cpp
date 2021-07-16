
#include "rodos.h"


static Application senderName("UDPSender");

static UDPOut out(5001);

class Sender : public StaticThread<> {
    char outbuf[200];

    void run () {
        int  cnt = 0;
        int portNr = 5001;
        PRINTF("to receive type : netcat -u -l 5001 \n");
        while(1) {
            cnt++;
            xsprintf(outbuf,"from UDPSender %d\n", cnt);
            PRINTF("Sending nr %d: %s\n", cnt, outbuf);
            out.send(outbuf, 200);
            AT(NOW()+500*MILLISECONDS);
            if((cnt % 15) == 0) {
                portNr++;
                PRINTF("Changing port to %d\n use now\n", portNr);
                PRINTF("netcat -u -l %d\n", portNr);
                out.reopen(portNr);
                suspendCallerUntil(NOW() + 3*SECONDS);
            }
        }
    }
};

static Sender   sender;
