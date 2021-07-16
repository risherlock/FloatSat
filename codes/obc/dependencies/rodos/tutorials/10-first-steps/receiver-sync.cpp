#include "rodos.h"
#include "topics.h"

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
