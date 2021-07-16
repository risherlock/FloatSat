#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 15", 1115);

struct JustPrint15 : public Subscriber {
    JustPrint15() : Subscriber(counter5, "justprint15") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R15, *static_cast<long*>(msg));
        return 1;
    }
} justPrint15;

