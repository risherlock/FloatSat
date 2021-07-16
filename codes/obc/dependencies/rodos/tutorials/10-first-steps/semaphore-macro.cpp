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
            PROTECT_IN_SCOPE(onlyOne);

            PRINTF(" only one, I am -- %c -- ", myId);
            yield(); // Just to show that the semaphore protects
            PRINTF("time %3.9f from %c\n", SECONDS_NOW(), myId); // Shall be the same from the first printf
        }
    }

} team[20];
