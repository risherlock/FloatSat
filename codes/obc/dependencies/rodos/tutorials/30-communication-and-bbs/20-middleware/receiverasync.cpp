#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application receiverName("ReciverAsync", 1300);

static Fifo<Position, 5> buf;
static Subscriber nameNotImportant01(position, buf, "position5");

class ReceiverAsync : public StaticThread<> {
  void run () {
    Position mypos;
    TIME_LOOP(20*SECONDS, 12*SECONDS) {
    	while(buf.get(mypos)) {
    		PRINTF(RX "%d" RY "%d" RZ "%d\n",(int)mypos.x, (int)mypos.y, (int)mypos.z);
    	}
    }
  }
} receiverAsync;

