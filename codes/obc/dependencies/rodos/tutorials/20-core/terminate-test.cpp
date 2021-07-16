#include "rodos.h"

#ifndef TIME_TO_END
#define TIME_TO_END 20
#endif

static Application terminateTestApp("terminateTest");

class TerminateTest : public StaticThread<> {
  public:
    TerminateTest() : StaticThread<>("TerminateTest", CEILING_PRIORITY) {}
    void run() {
        PRINTF("\nThis run (test) will be terminated in %d Seconds\n", TIME_TO_END);
        AT(TIME_TO_END*SECONDS);
        PRINTF("\nThis run (test) terminates now! (%s)\n", LOCATION);
        hwResetAndReboot();
    }
};

static TerminateTest terminateTest;
