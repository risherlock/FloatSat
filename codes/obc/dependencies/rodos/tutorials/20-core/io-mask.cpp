#include "rodos.h"
#include "io-mask.h"

void MAIN() {
    INIT_MASK();
    activateTopicCharInput(); //<........ THIS IS IT!!!!
}

/***** outpus ********/
class ThreadAA : public StaticThread<> {
  public:
    void run() {
        int cnt = 0;
        TIME_LOOP(1 * SECONDS, 1 * SECONDS) { PRINTF(CNTAA "%d" INPUTLINE, cnt++); }
    }
} aa;


class ThreadBB : public StaticThread<> {
  public:
    void run() {
        int cnt = 0;
        TIME_LOOP(3 * SECONDS, 500 * MILLISECONDS) { PRINTF(CNTBB "%d" INPUTLINE, cnt++); }
    }
} bb;

/********* Input *********/

class CharReceiver : public Subscriber {
  public:
    CharReceiver() : Subscriber(charInput, "CharReceiver") {}

    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, const void* data, [[gnu::unused]] size_t len) {
        const GenericMsgRef* msg = static_cast<const GenericMsgRef*>(data);
        xprintf(READ "                      ");
        xprintf(READ "%s\n", static_cast<char*>(msg->msgPtr)); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
