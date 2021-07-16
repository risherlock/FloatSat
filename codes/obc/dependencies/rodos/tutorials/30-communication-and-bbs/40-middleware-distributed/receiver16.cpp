#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 16", 1116);

struct JustPrint16 : public Subscriber {
    JustPrint16() : Subscriber(counter6, "justprint16") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R16, *static_cast<long*>(msg));
        return 1;
    }
} justPrint16;

