#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

/******************************/

static Application senderName("Publisher 15", 2015);

class MyPublisher15 : public StaticThread<> {
public:
    MyPublisher15() : StaticThread<>("sender15") { }
    void run () {
        long cnt = -30000;
        TIME_LOOP(0, 2300*MILLISECONDS) {
            PRINTF("%s%ld", S15, --cnt);
            counter5.publish(cnt);
        }
    }
} myPublisher15;

