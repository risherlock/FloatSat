#include "rodos.h"

static Application module01("YieldTime-extra");

class SimpleThread : public StaticThread<> {
    long long yieldCnt;
    void      run() {
        while(1) {
            yield();
            yieldCnt++;
            if((yieldCnt % 100000) == 0) {
                long long timeNow = NOW();
                PRINTF(" %s: cnt = %lld, totalyiels = %lld, Time = %3.9f  nsec/dispatchCnt = %ld\n",
                       getName(),
                       yieldCnt,
                       getScheduleCounter(),
                       (double)timeNow / SECONDS,
                       (long)(((double)timeNow / (double)getScheduleCounter()) * NANOSECONDS));
            }
        }
    }

  public:
    SimpleThread() : StaticThread<>("Simple") { yieldCnt = 0; }
    void init() { PRINTF(" Simple Thread activated"); }
};

/*****************************/

SimpleThread x[20];
