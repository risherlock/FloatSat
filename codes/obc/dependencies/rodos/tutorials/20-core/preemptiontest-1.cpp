#include "rodos.h"

/****** Use modulId 2000 just be be found by other example: ceiler **/

static Application module01("PreemptiveTest", 2000);

class LowPriorityThread : public StaticThread<> {
    char symbol;

  public:
    LowPriorityThread(char symbol) : StaticThread<>("LowPriority", 20) {
        this->symbol = symbol;
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
                xprintf("%c", symbol);
                FFLUSH();
            }
        }
    }
};


/******************/

LowPriorityThread lp1('.');
LowPriorityThread lp2('+');
LowPriorityThread lp3('-');
LowPriorityThread lp4('*');


/******************/
