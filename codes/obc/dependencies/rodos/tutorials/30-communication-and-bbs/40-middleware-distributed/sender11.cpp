#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"


/******************************/

static Application senderName("Publisher 11", 2011);

class MyPublisher11 : public StaticThread<> {
public:
    MyPublisher11() : StaticThread<>("sender11") { }
    void run () {
        long cnt = 30000;
        TIME_LOOP(0, 1000*MILLISECONDS) {
            PRINTF("%s%ld", S11, ++cnt);
            counter1.publish(cnt);
        }
    }
} myPublisher11;


