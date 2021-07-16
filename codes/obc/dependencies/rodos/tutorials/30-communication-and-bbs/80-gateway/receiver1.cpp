#include "rodos.h"
#include "demo_topics.h"

struct JustPrint1 : public Subscriber {
    JustPrint1() : Subscriber(counter1, "receiver1") { }
    uint32_t put(const uint32_t topicId, const size_t len, void* msg, const NetMsgInfo& netMsgInfo) {
        PRINTF("at %9.3f       counter1: got (topic, len, mesg, netinfo): %lu, %lu, %d, (%ld, %lld, %lu, %lu)\n",
            SECONDS_NOW(),
            static_cast<unsigned long>(topicId),
            static_cast<unsigned long>(len),
            static_cast<int>(*static_cast<int32_t*>(msg)),
            static_cast<long>(netMsgInfo.senderNode),
            static_cast<long long>(netMsgInfo.sentTime),
            static_cast<unsigned long>(netMsgInfo.senderThreadId),
            static_cast<unsigned long>(netMsgInfo.linkId));
        return true;
    }
} justPrint1;

