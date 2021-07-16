#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 14", 1114);

struct JustPrint14 : public Subscriber {
    JustPrint14() : Subscriber(counter4, "justprint14") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R14, *static_cast<long*>(msg));
        return 1;
    }
} justPrint14;

