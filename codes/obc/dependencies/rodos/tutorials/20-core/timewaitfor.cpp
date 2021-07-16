#include "rodos.h"

static Application module01("TestTime");

class TestTime : public StaticThread<> {
  public:
    TestTime() : StaticThread<>("waitfor") {}
    void run() {
        int cnt = 0;
        while(1) {
            cnt++;
            suspendCallerUntil(NOW() + 2 * SECONDS);
            PRINTF("After 2 Seconds  : %3.9f %d\n", SECONDS_NOW(), cnt);
        }
    }
    void init() { PRINTF("Waiting 2 seconds"); }
};

static TestTime testTime;
