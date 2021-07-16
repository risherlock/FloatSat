#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 13", 1113);

struct JustPrint13 : public Subscriber {
    JustPrint13() : Subscriber(counter3, "justprint13") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R13, *static_cast<long*>(msg));
        return 1;
    }
} justPrint13;

