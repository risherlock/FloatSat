#include "rodos.h"
#include "demo_topics.h"


class Receiver1 : public SubscriberReceiver<int32_t> {
public:
    Receiver1() : SubscriberReceiver<int32_t>(counter1, "receiver1") { }
    void put(int32_t &data) {
	    data = longConvertHost2Net(data); // changes back the byte order
        PRINTF("at %9.3f       counter1: got %d\n", SECONDS_NOW(), data);
    }
} simpleSub1;

