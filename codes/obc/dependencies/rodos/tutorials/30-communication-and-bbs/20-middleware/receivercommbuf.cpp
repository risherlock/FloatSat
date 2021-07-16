#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application receiverName("ReciverComBuffer", 1200);

static CommBuffer<Position> buf;
static Subscriber nameNotImportant02(position, buf, "receiverbuf");

class ReceiverBuf : public StaticThread<> {
  void run () {
    Position mypos;
    TIME_LOOP(10*SECONDS, 2*SECONDS) {
    	buf.get(mypos);
    	PRINTF( CX "%d" CY "%d" CZ "%d\n",(int)mypos.x, (int)mypos.y, (int)mypos.z);
    }
  }
} recbuf;

