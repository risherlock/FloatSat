#include "rodos.h"

/****** Use modulId 2000 just be be found by other example: ceiler **/

static Application module01("PreemptiveTest", 2000);

class HighPriorityThread01 : public StaticThread<> {
  public:
    HighPriorityThread01() : StaticThread<>("HiPriority", 25) {
    }

    void init() {
        xprintf(" hipri = '*'");
    }

    void run() {
        while(1) {
            xprintf("*");
            FFLUSH();
            suspendCallerUntil();
        }
    }
};

HighPriorityThread01 highPriorityThread01;


class LowPriorityThread : public StaticThread<> {
  public:
    LowPriorityThread() : StaticThread<>("LowPriority", 10) {
    }

    void init() {
        xprintf(" lopri = '.'");
    }

    void run() {
        long long cnt = 0;
        while(1) {
            cnt++;
            if(cnt % 1000000 == 0) {
                xprintf(".");
                FFLUSH();
            }
            if(cnt % 10000000 == 0) {
                highPriorityThread01.resume();
            }
        }
    }
};


/******************/

LowPriorityThread lowPriorityThread;

/******************/
