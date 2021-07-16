#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("TestTimebeats02");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        
        PRINTF("First beat in 0.03 seconds, period 0.02 seconds\n");

        int cnt = 0;
        
        TIME_LOOP(30 * MILLISECONDS, 20 * MILLISECONDS) {
            PRINTF("in 0.02 seconds beat: %3.9f\n", CNT_SECONDS_NOW());
            cnt++;
            if (cnt > 10) {
                hwResetAndReboot();
            }
        }
    }
};


static TestTimeBeat testTimeBeat;
