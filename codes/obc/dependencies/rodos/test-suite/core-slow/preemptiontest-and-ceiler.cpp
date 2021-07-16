#include "rodos.h"

uint32_t printfMask = 0;

/****** Use modulId 2000 just be be found by other example: ceiler **/

static Application module01("PreemptiveTest", 2000);

class HighPriorityThread : public StaticThread<> {
  public:
    HighPriorityThread() : StaticThread<>("HiPriority", 25) {
    }

    void init() {
        printfMask = 1;
        xprintf(" hipri = '*'\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;
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
        printfMask = 1;
        xprintf(" lopri = '.'\n");
        printfMask = 0;
    }

    void run() {
        printfMask = 1;
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

static Application module02("PriorityCeiling", 3000);

class PriorityCeiler : public StaticThread<> {
  public:
    PriorityCeiler() : StaticThread<>("PriotyCeiler", 10) {
    }

    void init() {
        printfMask = 1;
        xprintf(" ceiler = '+'\n");
        printfMask = 0;
    }


    void run() {
        printfMask = 1;
        suspendCallerUntil(3 * SECONDS);
        int64_t aproxLoopsFor2Seconds = getSpeedKiloLoopsPerSecond() * 1000LL;
        int64_t aproxLoopsForPrint    = aproxLoopsFor2Seconds / 80;

        xprintf("\nPriority ceiler 122 times normal\n");
        for(int64_t cnt = 0; cnt < aproxLoopsFor2Seconds; cnt++) {
            if(cnt % aproxLoopsForPrint == 0) {
                xprintf("-");
                FFLUSH();
            }
        }

        xprintf("\nPriority ceiler 122 tmes prio ceiling\n");
        {
            PRIORITY_CEILER_IN_SCOPE();
            for(int64_t cnt = 0; cnt < aproxLoopsFor2Seconds; cnt++) {
                if(cnt % aproxLoopsForPrint == 0) {
                    xprintf("-");
                    FFLUSH();
                }
            }
        }

        xprintf("\nPriority ceiler 122 times normal\n");
        for(int64_t cnt = 0; cnt < aproxLoopsFor2Seconds; cnt++) {
            if(cnt % aproxLoopsForPrint == 0) {
                xprintf("-");
                FFLUSH();
            }
        }


        xprintf("\nPriority ceiler 122 times normal\n");
        for(int64_t cnt = 0; cnt < aproxLoopsFor2Seconds; cnt++) {
            if(cnt % aproxLoopsForPrint == 0) {
                xprintf("-");
                FFLUSH();
            }
        }


        AT(NOW()+ 2*SECONDS);
        xprintf("\n Test terminaes\n");
        hwResetAndReboot ();
        
    }
};


/******************/

PriorityCeiler priorityCeiler;

/******************/

void MAIN() {
    printfMask = 1;
    if(Application::findApplication(2000) == 0) {
        xprintf("\n\n**********************\n");
        xprintf("To be able to test it compile together with PreemptionTest\n");
        xprintf("**********************\n\n\n");
    } else {
        xprintf("Preemptiontest found\n");
    }
}


