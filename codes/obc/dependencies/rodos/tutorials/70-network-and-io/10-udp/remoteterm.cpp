
#include "rodos.h"

static Application  receiverName("remoteterm");

static UDPIn in(-6001); // negative to allow several to listen the same port
static UDPOut out(-6001); // Negative port means broadcast to this port

class Term : public StaticThread<> {
    char userData[2000];
    char *s;
    uint32_t senderIp;
    void run () {
        PRINTF("my node nr = %ld\n", static_cast<long>(getNodeNumber()));
        TIME_LOOP(1*SECONDS, 100*MILLISECONDS) {
            while(int len = in.get(userData, 200, &senderIp)) {
                userData[len] = 0;
                PRINTF("from %lx %s\n",
                    static_cast<unsigned long>(senderIp),
                    userData);
            }
            while((s = getsNoWait() ) != 0 ) {
                size_t len = strlen(s);
                if(len > UINT16_MAX) len = UINT16_MAX;
                out.send(s, static_cast<uint16_t>(len));
            }
        } // TIME_LOOP
    } // run
} term;


