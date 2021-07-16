#include "rodos.h"
#include "demo_topics.h"

uint32_t printfMask = 0;

class MyPublisher11 : public StaticThread<> {
public:
    MyPublisher11() : StaticThread<>("sender11") { }
    void run () {
        printfMask = 1;
        int32_t cnt32      = 100;
        int64_t cnt64      = 100000;
        double  cntDouble  = 1.0;

        AT(1*SECONDS);
        PRINTF("sending 3 topics:\n");
        for(int i = 0; i < 1000; i++) {
            cnt32++;
            cnt64++;
            cntDouble += 0.01;
            PRINTF("  %4d  %8lld  %3.2f\n", cnt32, static_cast<long long>(cnt64), cntDouble);

            int32top.publish(cnt32);
            int64top.publish(cnt64);
            doubltop.publish(cntDouble);

     }
     AT(2*SECONDS);
     /** Signal to the receivers to termimate **/
     cnt32     = -1;
     cnt64     = -1;
     cntDouble = -1.0;
     PRINTF("terminate Signal:  %4d  %8lld  %3.2f\n", cnt32, static_cast<long long>(cnt64), cntDouble);
     int32top.publish(cnt32);
     int64top.publish(cnt64);
     doubltop.publish(cntDouble);

     PRINTF("Sender terminates\n");
     hwResetAndReboot();
    }
} myPublisher11;


