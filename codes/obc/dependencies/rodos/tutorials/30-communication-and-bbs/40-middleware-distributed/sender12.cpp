#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

/******************************/

static Application senderName("Publisher 12", 2012);

class MyPublisher12 : public StaticThread<> {
public:
    MyPublisher12() : StaticThread<>("sender12") { }
    void run () {
        long cnt = 20000;
        TIME_LOOP(0,  1300*MILLISECONDS) {
            PRINTF("%s%ld", S12, ++cnt);
            counter2.publish(cnt);
        }
    }
} myPublisher12;

