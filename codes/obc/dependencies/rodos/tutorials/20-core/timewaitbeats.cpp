#include "rodos.h"

static Application module01("TestTime02");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        int cnt = 0;
        setPeriodicBeat(3 * SECONDS, 2 * SECONDS);
        PRINTF("First beat in 3 seconds, period 2 seconds\n");
        while(1) {
            cnt++;
            suspendUntilNextBeat();
            PRINTF("in 2 seconds beat: %3.9f %d\n", SECONDS_NOW(), cnt);
        }
    }
};


static TestTimeBeat testTimeBeat;
