#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("ReciverSync", 1400);

static SyncFifo<long, 5> fifo;
static Subscriber nameNotImportant02(counter1, fifo, "fifo");

class ReceiverSync : public StaticThread<> {
  void run () {
    long cnt = 0;
    while(1) {
      fifo.syncGet(cnt);
      PRINTF(SC "%ld\n", cnt);
    }
  }
} receiverSync;

