#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 12", 1112);

struct JustPrint12 : public Subscriber {
    JustPrint12() : Subscriber(counter2, "justprint12") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld", R12, *static_cast<long*>(msg));
        return 1;
    }
} justPrint12;
