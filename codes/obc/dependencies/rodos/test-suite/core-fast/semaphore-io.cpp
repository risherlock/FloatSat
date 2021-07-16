#include "rodos.h"

uint32_t printfMask = 0;

/****** Use modulId 2000 just be be found by other example: ceiler **/

void veryLowSpeedPrinter(const char* text) {
    int32_t intervalToPrint = getSpeedKiloLoopsPerSecond() * 10;
    int64_t cnt             = 0;
    for(const char* cptr = text; *cptr != 0;) {
        cnt++;
        if(cnt % intervalToPrint == 0) {
            xprintf("%c", *cptr);
            cptr++;
            FFLUSH();
        }
    }
}


Semaphore protector;

class TestThread : public StaticThread<> {
    void run() {
        printfMask = 1;
        PRINTF("remove the semaphore and try again!\n");
        protector.enter();
        {
            veryLowSpeedPrinter("Hello world, I am here\n");
        }
        protector.leave();
        AT(NOW() + 2*SECONDS);
        hwResetAndReboot();
    }
};

TestThread t1, t2;
