#include "rodos.h"

static Application module01("Testgetchar");

class TestGets : public StaticThread<> {
  public:
    TestGets() : StaticThread<>("testgetchar") {}
    void run() {
        char* s;
        PRINTF("Please type string of characters. Run at least 40 seconds\n");

        activateTopicCharInput(); //<........ THIS IS IT!!!!

        TIME_LOOP(30 * SECONDS, 1 * SECONDS) {
            PRINTF("I call getsnowait -> topic deactivated! \n");
            if((s = getsNoWait()) != 0) { PRINTF("\n\n********* String: %s'%s'%s **********\n", SCREEN_GREEN, s, SCREEN_RESET); }
        }
    }
} testGets;

/******************************/

class CharReceiver : public Subscriber {
  public:
    CharReceiver() : Subscriber(charInput, "CharReceiver") {}

    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, const void* data, [[gnu::unused]] size_t len) {
        const GenericMsgRef* msg       = static_cast<const GenericMsgRef*>(data);
        char content[100] = {};
        memcpy(content, msg->msgPtr, static_cast<size_t>(msg->msgLen > 100 ? 100 : msg->msgLen - 1));
        content[msg->msgLen > 100 ? 99 : msg->msgLen] = 0;
        xprintf("\n Async: %u %s\n", static_cast<unsigned int>(msg->msgLen), content); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
