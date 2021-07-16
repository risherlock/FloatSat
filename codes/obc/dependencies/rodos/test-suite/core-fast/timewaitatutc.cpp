#include "rodos.h"

uint32_t printfMask = 0;

static Application module02("TestTimeAT");

static class TestTime : public StaticThread<> {

  public:
    TestTime() : StaticThread<>("waitAT") {}
    void run() {
        printfMask = 1;
        sysTime.setUTC(100 * SECONDS);
        PRINTF("Starting and waiting until 1st second\n");
        AT_UTC(101 * SECONDS);
        PRINTF("After 1st second\n");
        hwResetAndReboot();
    }
} testTime;
