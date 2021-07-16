#include "rodos.h"

/****** Use modulId 2000 just be be found by other example: ceiler **/

static Application module01("PreemptiveTest", 2000);

class HighPriorityThread : public StaticThread<> {
  public:
    HighPriorityThread() : StaticThread<>("HiPriority", 25) {
    }

    void init() {
        xprintf(" hipri = '*'");
    }

    void run() {
        while(1) {
            xprintf("*");
            FFLUSH();
            suspendCallerUntil(NOW() + 500 * MILLISECONDS);
        }
    }
};


class LowPriorityThread : public StaticThread<> {
  public:
    LowPriorityThread() : StaticThread<>("LowPriority", 10) {
    }

    void init() {
        xprintf(" lopri = '.'");
    }

    void run() {
        volatile int64_t cnt             = 0;
        int64_t          intervalToPrint = getSpeedKiloLoopsPerSecond() * 10;
        while(1) {
            cnt++;
            if(cnt % intervalToPrint == 0) {
                xprintf(".");
                FFLUSH();
            }
        }
    }
};


/******************/

HighPriorityThread highPriorityThread;
LowPriorityThread  lowPriorityThread;

/******************/
