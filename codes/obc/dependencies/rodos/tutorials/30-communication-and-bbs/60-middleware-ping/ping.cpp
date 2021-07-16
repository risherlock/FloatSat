#include "rodos.h"
#include "mytopics.h"


/******************************/

static Application senderName("Publisher ping", 2011);

int64_t tSend, tReceive;
class PingSender : public StaticThread<>, Subscriber {
public:
   PingSender(): StaticThread<>("ppingsender"), Subscriber (pong, "pongReceiver") { }
   void run () {
	long pingCnt = 0;
	TIME_LOOP(0, 1*SECONDS) {
	   PRINTF("ping %ld at %lld\n", pingCnt, static_cast<long long>(NOW()));
      tSend = NOW();
	   ping.publish(pingCnt);
      pingCnt++;
	}
   }

   uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* data, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
         tReceive = NOW();
         long cnt = *(long*)data;
         PRINTF("pong %ld at %lld, dif = %6.3f us\n", cnt, static_cast<long long>(tReceive), (double)(tReceive - tSend)/MICROSECONDS);
         return 1;
   }

} pingSender;
