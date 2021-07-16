#include "rodos.h"
#include "demo_topics.h"


class Receiver2 : public SubscriberReceiver<int32_t> {
public:
    Receiver2() : SubscriberReceiver<int32_t>(counter2, "receiver2") { }
    void put(int32_t &data) {
        PRINTF("at %9.3f       counter2: got %d\n", SECONDS_NOW(), static_cast<int>(data));
    }
} simpleSub2;

