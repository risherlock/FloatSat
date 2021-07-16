#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("resumefromEvent");

class TestWaiter : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        for (int i = 0; i < 10; i++) {
            PRINTF("Suspend and wait until some one resumes me\n");
            suspendCallerUntil();
            //double t0 = SECONDS_NOW();
            PRINTF("testwaiter running again at %3.9f\n", CNT_SECONDS_NOW());
        }
        hwResetAndReboot();
    }
};


static TestWaiter testWaiter;


class TimeEventTest : public TimeEvent {
  public:
    void handle() {
        xprintf("   Time Event at %3.9f\n", CNT_SECONDS_NOW());
        testWaiter.resume();
        xprintf("   Testwaiter resumed from me\n");
    }

    void init() { activatePeriodic(50 * MILLISECONDS, 30 * MILLISECONDS); }
};

static TimeEventTest te01;
