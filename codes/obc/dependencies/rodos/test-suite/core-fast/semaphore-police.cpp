
#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

Semaphore onlyOne;
int       globalCnt = 0;

class Watcher : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        onlyOne.enter();
        int myId = globalCnt++;
        onlyOne.leave();
        
        int i = 0;

        TIME_LOOP(30 * MILLISECONDS, 20 * MILLISECONDS) {
            i++;
            if (i > 5) {
                hwResetAndReboot();
            }
            onlyOne.enter();
            PRINTF(" only one, I am -- %02d -- ,", myId);
            yield(); // Just to show that the semaphore protects
            PRINTF("time %3.9f\n", CNT_SECONDS_NOW());
            onlyOne.leave();
        }
        
    }
} team[20];
