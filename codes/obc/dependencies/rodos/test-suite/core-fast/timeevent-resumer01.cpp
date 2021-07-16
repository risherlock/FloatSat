#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module01("TestTimebeats & events 01");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        PRINTF("First beat in 0.05 seconds, period 0.05 seconds\n");
        int i = 0;
        TIME_LOOP(50*MILLISECONDS, 50*MILLISECONDS) {
            i++;
            if (i > 10) {
                hwResetAndReboot();
            }
            int t0 = static_cast<int>(CNT_SECONDS_NOW());
            PRINTF("In 0.05 seconds beat: %d \n", t0);
        }
    }
};
static TestTimeBeat testTimeBeat;


class Resumer : public TimeEvent {
  protected:
    void handle() {
        int t0 = static_cast<int>(CNT_SECONDS_NOW());
        xprintf("Time Event at %d -> resuming\n", t0);
        testTimeBeat.resume();
        if(NOW() < 50 * MILLISECONDS) activateAt(NOW() + 20*MILLISECONDS);
    }

    void init() {
        activateAt(10 * SECONDS);
    }
};
static Resumer resumer;
