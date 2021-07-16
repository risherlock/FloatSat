
#include "rodos.h"

Semaphore onlyOne;
int       globalCnt = 0;

class Watcher : public StaticThread<> {
  public:
    void run() {
        onlyOne.enter();
        int myId = globalCnt++;
        onlyOne.leave();

        TIME_LOOP(3 * SECONDS, 2 * SECONDS) {
            onlyOne.enter();
            PRINTF(" only one, I am -- %02d -- ,", myId);
            yield(); // Just to show that the semaphore protects
            PRINTF("time %3.9f\n", SECONDS_NOW());
            onlyOne.leave();
        }
    }
} team[20];
