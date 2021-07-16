
#include "rodos.h"
#include "demo_topics.h"

/******************************/

class MyPublisher : public StaticThread<> {
public:
    MyPublisher() : StaticThread<>("sender") { }
    void run () {
        int32_t cnt1 = 1000;
        int32_t tmp;
        TIME_LOOP(0, 1000*MILLISECONDS) {
            PRINTF("at %9.3f sending %d\n",  SECONDS_NOW(), cnt1);
            tmp = longConvertHost2Net(cnt1);
            counter1.publish(tmp);
            cnt1++;
        }
    }
} myPublisher;


