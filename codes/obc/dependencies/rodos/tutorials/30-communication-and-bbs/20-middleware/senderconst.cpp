#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

/******************************/

static Application senderName("Publisher 01 const", 1100);

#define CNT 100L

class MyPublisherC : public StaticThread<> {
public:
	MyPublisherC() : StaticThread<>("SenderConst") { }
	void run () {
		TIME_LOOP(3*SECONDS, 3*SECONDS) {
			PRINTF(CONST "%lld\n", static_cast<long long>(NOW()));
			PRINTF(CNT2 "%ld\n", CNT);
			counter2.publish(CNT);
		}
	}
} publisherC;


