#include "rodos.h"

static Application module01("deepsleepmode");

class TestTimeBeat : public StaticThread<> {
  public:
    void run() {
        TIME_LOOP(1 * SECONDS, 1 * SECONDS) {
            PRINTF("Time now: %3.9f\n", SECONDS_NOW());
        }
    }
} testTimeBeat;

class Speeler : public StaticThread<> {
  public:
    void run() {
        PRINTF("In 5 Secons, Sleep for 10 Seconds\n");
        AT(5 * SECONDS);
        PRINTF("Deepsleep now\n");
        deepSleepUntil(15 * SECONDS);
        PRINTF("Wake up\n");
    }
} sleeper;
