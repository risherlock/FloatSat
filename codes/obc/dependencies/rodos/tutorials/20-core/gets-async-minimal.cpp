#include "rodos.h"

void MAIN() {
    PRINTF("please write some lines\n");
    activateTopicCharInput(); //<........ THIS IS IT!!!!
}


class CharReceiver : public Subscriber {
  public:
    CharReceiver() : Subscriber(charInput, "CharReceiver") {}

    void putFromInterrupt([[gnu::unused]] const uint32_t topicId, const void* data, [[gnu::unused]] size_t len) {
        const GenericMsgRef* msg = static_cast<const GenericMsgRef*>(data);
        xprintf("\n Async: %u %s\n", static_cast<unsigned int>(msg->msgLen), static_cast<char*>(msg->msgPtr)); // no PRINTF in interrupts (Sempahore)
    }
} charReceiver;
