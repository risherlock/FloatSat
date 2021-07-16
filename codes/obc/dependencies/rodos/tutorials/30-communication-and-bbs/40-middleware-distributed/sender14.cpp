#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

/******************************/

static Application senderName("Publisher 14", 2014);

class MyPublisher14 : public StaticThread<> {
public:
    MyPublisher14() : StaticThread<>("sender14") { }
    void run () {
        long cnt = 0;
        TIME_LOOP(0,  1900*MILLISECONDS) {
            PRINTF("%s%ld", S14, ++cnt);
            counter4.publish(cnt);
        }
    }
} myPublisher14;

