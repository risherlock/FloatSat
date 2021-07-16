#include "rodos.h"
#include "activity.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static int cnt = 0;

class A1 : public Activity {
public:
    A1() : Activity("A1", 200, 50*MILLISECONDS, 50*MILLISECONDS) { }
    void step([[gnu::unused]] int64_t timeNow) {
        printfMask = 1;
        PRINTF("A1 %4.6f\n", CNT_SECONDS_NOW());
        cnt++;
        if (cnt > 10) {
            hwResetAndReboot();
        }
    }
} a1;

class A2 : public Activity {
public:
    A2() : Activity("A2") { }
    void init() { activatePeriodic(20*MILLISECONDS, 30*MILLISECONDS); }
    void step([[gnu::unused]] int64_t timeNow) {
        printfMask = 1;
        PRINTF("A2 %4.6f\n", CNT_SECONDS_NOW());
    }
} a2;

