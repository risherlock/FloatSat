#include "rodos.h"

uint32_t printfMask = 0;

static Application module01("Testgetchar");

class TestGets : public StaticThread<> {
  public:
    TestGets() : StaticThread<>("testgetchar") {}
    void run() {
        printfMask = 1;
        char* s;
        PRINTF("Please type string of characters. Run at least 0.04 seconds\n");

        activateTopicCharInput(); //<........ THIS IS IT!!!!

        int cnt = 0;
        
        TIME_LOOP(300 * MILLISECONDS, 1 * SECONDS) {
            cnt++;
            if (cnt > 10) {
                PRINTF("-------------------------------------------------------------");
                hwResetAndReboot();
            }
            
            PRINTF("I call getsnowait -> topic deactivated! \n");
            if((s = getsNoWait()) != 0) { PRINTF("\n\n********* String: '%s' **********\n", s); }
        }
    }
} testGets;

/******************************/

class CharReceiver : public Subscriber {
  public:
    CharReceiver() : Subscriber(charInput, "CharReceiver") {}

    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, const void* data, [[gnu::unused]] size_t len) {
        const GenericMsgRef* msg       = static_cast<const GenericMsgRef*>(data);
        xprintf("\n Async: %d ", (int)msg->msgLen); // no PRINTF in interrupts (Sempahore)
        for(int i = 0; i < msg->msgLen; i++) {
            xprintf("%c", static_cast<const char*>(msg->msgPtr)[i]);
        }
        xprintf("\n");
    }
} charReceiver;
