
#include "rodos.h"

static Application  receiverName("UDPReceiverAsync");

Topic<GenericMsgRef> udpMessages(-1, "udpMessages");

static UDPIn in(5001);
static UDPIn nameNotImportatn01(5002, &udpMessages);
static UDPIn nameNotImportatn02(5003, &udpMessages);

void MAIN() {
    PRINTF("\n\n to test please type in other terminal\n");
    PRINTF("    netcat -u localhost 5001\n");
    PRINTF("    netcat -u localhost 5002\n");
    PRINTF("    netcat -u localhost 5003\n");
}

class ReceiverPolling : public StaticThread<> {
    char userData[2000];
    void run () {
        TIME_LOOP(1*SECONDS, 1*SECONDS) {
            PRINTF("\nPolling:");
            while(int len = in.get(userData, 2000)) {
                PRINTF("from polling: %d %s\n", len, userData);
            }
        }
    }
} receiverPolling;

/******************************/

class Udpreceiver :  public Subscriber {
public:
    Udpreceiver() : Subscriber(udpMessages, "udpReceiver") { }

    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, const void* data, [[gnu::unused]] size_t len) {
        const GenericMsgRef* msg = (const GenericMsgRef*)data;
        char buffer[100] = {};
        memcpy(buffer, msg->msgPtr, static_cast<size_t>(msg->msgLen > 100 ? 100 : msg->msgLen));
        buffer[msg->msgLen > 100? 99 : msg->msgLen - 1] = 0;
        xprintf("\n Async: %d %s\n", static_cast<int>(msg->msgLen), static_cast<char*>(msg->msgPtr)); // no PRINTF in interrupts (Sempahore)
    }
} udpReceiver;


