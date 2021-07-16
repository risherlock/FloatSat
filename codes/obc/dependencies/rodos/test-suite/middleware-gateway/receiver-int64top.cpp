#include "rodos.h"
#include "demo_topics.h"

uint32_t printfMask = 0;

struct PrintInt64Top : public Subscriber {
    PrintInt64Top() : Subscriber(int64top, "int64top-printer") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        printfMask = 1;
        int64_t val = *static_cast<int64_t*>(msg);
        PRINTF("int64top: %8lld\n", static_cast<long long>(val));
        if(val < 0) {
            PRINTF("signal to terminate\n");
            hwResetAndReboot();
        }
        return true;
    }
} printInt64Top;


