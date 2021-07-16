#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

/******************************/

static Application senderName("Publisher 03 Position", 1300);
static Position myPos;

class MyPublisher03 : public StaticThread<> {
public:
	MyPublisher03() : StaticThread<>("SenderPosition") { }
	void run () {
		long cnt = 10000;
		TIME_LOOP(3*SECONDS, 3*SECONDS) {
			PRINTF(PX "%ld" PY "%ld" PZ "%ld\n", cnt, cnt, cnt);
			myPos.x = static_cast<float>(cnt++);
			myPos.y = static_cast<float>(cnt++);
			myPos.z = static_cast<float>(cnt++);
			position.publish(myPos);
		}
	}
} publisher03;

