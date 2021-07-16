#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static Application module02("TestTimeAT");

static class TestTime : public StaticThread<> {

  public:
    TestTime() : StaticThread<>("waitAT") {}

    void run() {
        printfMask = 1;
        
        PRINTF("Waiting until 0.01 seconds after start\n");
        AT(10 * MILLISECONDS);
        PRINTF("After 0.01 seconds\n");

        PRINTF("Waiting until 0.015 seconds have passed\n");
        AT(NOW() + 15 * MILLISECONDS);
        PRINTF("0.025 second has passed\n");

        PRINTF("Print every 0.02 seconds, start at 0.04 seconds\n");
        
        int cnt = 0;
        
        TIME_LOOP(15 * MILLISECONDS, 20 * MILLISECONDS) {
            PRINTF("Current time: %3.9f\n", CNT_SECONDS_NOW());     //CNT_SECONDS_NOW only increases a counter, NO real time
            cnt++;
            if (cnt > 10) {
                hwResetAndReboot();
            }
        }
    }

    void init() { PRINTF("Waiting time"); }

} testTime;
