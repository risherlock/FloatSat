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
 

static Application receiverName("ReciverSync", 1400);

static SyncFifo<long, 5> fifo;
static Subscriber        nameNotImportant02(counter1, fifo, "fifo");

class ReceiverSync : public StaticThread<> {
    void run() {
        long cnt = 0;
        while(1) {
            fifo.syncGet(cnt);
            PRINTF("ReceiverSync - counter1: %ld\n", cnt);
        }
    }
} receiverSync;
