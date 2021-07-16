#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module02("TestTimeAT");

static class TestTime : public StaticThread<> {

  public:
    TestTime() : StaticThread<>("waitAT") {}
    void run() {
        printfMask = 1;
        
        int cnt = 0;

        PRINTF("Starting and waiting until 0.03 second\n");
        AT(30 * MILLISECONDS);
        PRINTF("After 0.03 second\n");

        for (int i = 0; i < 10; i++) {
            cnt++;
            AT(NOW() + 20 * MILLISECONDS);
            PRINTF("After 0.02 Seconds  : %3.9f %d\n", CNT_SECONDS_NOW(), cnt);
        }
        hwResetAndReboot();
    }

    void init() { PRINTF("Waiting 2 seconds"); }
} testTime;
