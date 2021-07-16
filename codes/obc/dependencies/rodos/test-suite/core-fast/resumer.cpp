#include "rodos.h"

uint32_t printfMask = 0;

/****** Use modulId 2000 just be be found by other example: ceiler **/

static Application module01("PreemptiveTest", 2000);

class HighPriorityThread01 : public StaticThread<> {
  public:
    HighPriorityThread01() : StaticThread<>("HiPriority", 25) {
    }

    void init() {
        printfMask = 1;
        PRINTF(" hipri = '*'\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;
        for (int i = 0; i < 100; i++) {
            PRINTF("*");
            FFLUSH();
            suspendCallerUntil();
        }
    }
};

HighPriorityThread01 highPriorityThread01;


class LowPriorityThread : public StaticThread<> {
  public:
    LowPriorityThread() : StaticThread<>("LowPriority", 10) {
    }

    void init() {
        printfMask = 1;
        PRINTF(" lopri = '.'\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;
        long long cnt = 0;
        for (int i = 0; i < 200; i++) {
            cnt++;
            if(cnt % 5 == 0) {
                PRINTF(".");
                FFLUSH();
            }
            if(cnt % 11 == 0) {
                highPriorityThread01.resume();
            }
        }
        PRINTF("\n");
        hwResetAndReboot();
    }
};


/******************/

LowPriorityThread lowPriorityThread;

/******************/
