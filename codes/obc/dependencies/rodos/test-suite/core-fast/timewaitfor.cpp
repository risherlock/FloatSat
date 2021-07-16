#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("TestTime");

class TestTime : public StaticThread<> {
  public:
    TestTime() : StaticThread<>("waitfor") {}
    void run() {
        printfMask = 1;
        int cnt = 0;
        for (int i = 0; i < 10; i++) {
            cnt++;
            suspendCallerUntil(NOW() + 100 * MILLISECONDS);
            PRINTF("After 0.1 Seconds  : %3.9f %d\n", CNT_SECONDS_NOW(), cnt);
        }
        hwResetAndReboot();
    }
    void init() { PRINTF("Waiting 0.1 seconds"); }
};

static TestTime testTime;
