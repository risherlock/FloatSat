#include "rodos.h"

Semaphore onlyOne;
char      globalCnt = 'A';

class Watcher : public StaticThread<> {
  public:
    void run() {

        char myId;

        //distribute the ids
        onlyOne.enter();
        myId = globalCnt++;
        onlyOne.leave();

        while(1) {
            onlyOne.enter();
            PRINTF(" only one, I am -- %c -- ", myId);
            yield(); // Just to show that the semaphore protects
            PRINTF("time %3.9f\n", SECONDS_NOW());
            onlyOne.leave();
        }
    }

} team[20];
