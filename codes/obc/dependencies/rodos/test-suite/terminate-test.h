#ifndef TERMINATE_TEST_H
#define TERMINATE_TEST_H 1

#include "rodos.h"

#ifndef TIME_TO_END
#define TIME_TO_END 4
#endif

#include "prt-seconds-now.h"


static Application terminateTestApp("terminateTest");

class TerminateTest : public StaticThread<> {
  public:
    TerminateTest() : StaticThread<>("TerminateTest", CEILING_PRIORITY) {}
    void run() {
        PRINTF("\nThis run (test) will be terminated in %d Seconds\n", TIME_TO_END);
        AT(TIME_TO_END*SECONDS);
        PRINTF("\nThis run (test) terminates now! but first error log:\n");
        errorLog.printAll();
        PRINTF("\nThis run (test) terminates now!\n");
        hwResetAndReboot();
    }
};

static TerminateTest terminateTest;


#endif // TERMINATE_TEST_H
