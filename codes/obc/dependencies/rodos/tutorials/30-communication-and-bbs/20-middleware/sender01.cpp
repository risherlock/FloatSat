#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

/******************************/

static Application senderName("Publisher 01 simple", 1100);

class MyPublisher01 : public StaticThread<> {
public:
	MyPublisher01() : StaticThread<>("SenderSimple") { }

	void run () {
		long cnt = 0;
		TIME_LOOP(3*SECONDS, 3*SECONDS) {
			PRINTF(CNT1 "%ld\n", ++cnt);
			counter1.publish(cnt);
		}
	}
} publishier01;


