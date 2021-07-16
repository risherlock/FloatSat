#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("deepsleepmode");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        TIME_LOOP(10 * MILLISECONDS, 10 * MILLISECONDS) {
            PRINTF("Time now: %3.9f\n", CNT_SECONDS_NOW());
        }
    }
} testTimeBeat;

class Sleeper : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        PRINTF("In 0.05 Seconds, Sleep for 0.10 Seconds\n");
        AT(50 * MILLISECONDS);
        PRINTF("Deepsleep now\n");
        deepSleepUntil(150 * MILLISECONDS);
        PRINTF("Wake up\n");
        hwResetAndReboot();
    }
} sleeper;
