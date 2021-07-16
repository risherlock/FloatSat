#include "rodos.h"

static Application module01("Priority");

class HighPriorityThread : public StaticThread<> {
  public:
    HighPriorityThread() : StaticThread<>("HiPriority", 25) {}

    void init() {
        PRINTF(" hipri = '*'");
    }

    void run() {
        while(1) {
            PRINTF("*");
            AT(NOW() + 1 * SECONDS);
        }
    }
};


class LowPriorityThread : public StaticThread<> {
  public:
    LowPriorityThread() : StaticThread<>("LowPriority", 10) {}

    void init() {
        PRINTF(" lopri = '.'");
    }

    void run() {
        long long cnt = 0;
        while(1) {
            cnt++;
            if(cnt % 10000000 == 0) {
                PRINTF(".");
            }
        }
    }
};

HighPriorityThread highPriorityThread;
LowPriorityThread  lowPriorityThread;
