#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("TestTime02");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        int cnt = 0;
        setPeriodicBeat(30 * MILLISECONDS, 20 * MILLISECONDS);
        PRINTF("First beat in 0.03 seconds, period 0.02 seconds\n");
        for (int i = 0; i < 10; i++) {
            cnt++;
            suspendUntilNextBeat();
            PRINTF("In 0.02 seconds beat: %3.9f %d\n", CNT_SECONDS_NOW(), cnt);
        }
        hwResetAndReboot();
    }
};


static TestTimeBeat testTimeBeat;
