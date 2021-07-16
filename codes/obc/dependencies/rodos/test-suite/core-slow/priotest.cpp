#include "rodos.h"

uint32_t printfMask = 0;
/****** Use modulId 2000 just be be found by other example: ceiler **/

#define LOOPS 100000000

static Application module01("priorityTest", 3000);


class HiPriorityThread : public StaticThread<> {
  public:
    HiPriorityThread() : StaticThread<>("HiPriority", 100) {
    }

    void init() {
        printfMask = 1;
        xprintf(" hipri\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;
        long long loopCnt = 0;
        long long cnt     = 0;
        while(cnt < LOOPS * 10) {
            cnt++;
            if(cnt % LOOPS == 0) {
                loopCnt++;
                PRINTF("  hi pri cnt = %lld\n", loopCnt);
            }
        }
        hwResetAndReboot();
    }
} hipri1, hipri2;


/******************/

class LowPriorityThread : public StaticThread<> {
  public:
    LowPriorityThread() : StaticThread<>("LowPriority", 30) {
    }

    void init() {
        printfMask = 1;
        xprintf(" lopri\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;
        long long loopCnt = 0;
        long long cnt     = 0;
        while(cnt < LOOPS * 10) {
            cnt++;
            if(cnt % LOOPS == 0) {
                loopCnt++;
                PRINTF(" low pri cnt = %lld\n", loopCnt);
            }
        }
    }
} lopri1, lopri2;
