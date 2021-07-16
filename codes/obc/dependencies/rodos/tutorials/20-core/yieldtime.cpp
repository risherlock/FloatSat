#include "rodos.h"

static Application module01("YieldTimeMeassure");

Semaphore printProtect;

long long yieldGlobal = 0;
class TestThread : public StaticThread<> {
    long long yieldCnt;
    void      run() {
        while(1) {
            yield();
            yieldCnt++;
            yieldGlobal++;
            // displays every 100000 times
            if((yieldCnt % 100000) == 0) {
                long long timeNow = NOW();
                printProtect.enter();
                PRINTF(" %s: ", getName());
                PRINTF("cnt = %ld, ", (long)yieldCnt);
                PRINTF("totalyiels = %ld, ", (long)getScheduleCounter());
                PRINTF("Time = %3.9f, ", (double)timeNow / SECONDS);
                PRINTF("nsec/dispatchCnt = %ld  ", (long)(((double)timeNow / (double)getScheduleCounter()) * NANOSECONDS));
                PRINTF("nsec/yieldCnt = %ld\n", (long)(((double)timeNow / (double)yieldGlobal) * NANOSECONDS));
                printProtect.leave();
            }
        }
    }
    //constructor
  public:
    TestThread(const char* name = "xx") : StaticThread<>(name) { yieldCnt = 0; }

    void init() { PRINTF(" Thread activated"); }
};

/*****************************/
TestThread a("A(1)"), b("B(2)"), c("C(3)");
