#include "rodos.h"
#include "demo_topics.h"
#include "mask.h"

static Application  receiverName("receiver 11", 1111);

struct JustPrint11 : public Subscriber {
    JustPrint11() : Subscriber(counter1, "justprint11") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("%s%ld",  R11, *static_cast<long*>(msg));
        PRINTF("%s%lld", MSGCNT, static_cast<long long>(getNumberOfReceivedMsgsFromNetwork()));
        return 1;
    }
} justPrint11;

