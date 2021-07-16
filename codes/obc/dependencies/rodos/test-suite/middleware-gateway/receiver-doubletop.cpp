#include "rodos.h"
#include "demo_topics.h"

uint32_t printfMask = 0;

struct PrintDoubletop : public Subscriber {
    PrintDoubletop() : Subscriber(doubltop, "doubltop-printer") { }
    uint32_t put([[gnu::unused]] const uint32_t topicId, [[gnu::unused]] const size_t len, void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        printfMask = 1;
        double  val = *static_cast<double*>(msg);
        PRINTF("doubltop: %4.2f\n", val);
        if(val < 0) {
            PRINTF("signal to terminate\n");
            hwResetAndReboot();
        }
        return true;
    }
} printDoubletop;


