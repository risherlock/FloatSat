#include "rodos.h"

uint32_t printfMask = 0;

Topic<long> counter1(-1, "counter1");

static Application senderName("Publisher 01 simple", 1100);

class MyPublisher01 : public StaticThread<> {
  public:
    MyPublisher01() : StaticThread<>("SenderSimple") {}

    void run() {
        printfMask = 1;
        long cnt = 0;
        TIME_LOOP(30 * MILLISECONDS, 30 * MILLISECONDS) {
            if (cnt > 10) {
                hwResetAndReboot();
            }
            PRINTF("Publisher01 sending Counter1 %ld\n", ++cnt);
            counter1.publish(cnt);
        }
    }
} publisher01;
 

static Application receiverName("ReciverPutter", 1100);

class JustPrint : public Putter {

    /// Implements the complete generic interface of putter
    bool putGeneric(const uint32_t topicId, size_t msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("ReceiverPutter -  msgLen: %lu data: %ld topicId: %lu\n",
            static_cast<unsigned long>(msgLen),
            *static_cast<const long*>(msg),
            static_cast<unsigned long>(topicId));
        return true;
    }

} justPrint;

static Subscriber nameNotImportant01(counter1, justPrint, "justprint01");
