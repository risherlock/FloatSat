#include "rodos.h"
#include "random.h"

static Application module("semaphore-deadlock");

Semaphore s1, s2;


class Requester : public StaticThread<> {
    char id;

  public:
    Requester(char name) {
        id = name;
    }

    void run() {
        bool s1Lock = false;
        bool s2Lock = false;
        PRINTF("Deadlock will come...\n");

        TIME_LOOP(0, 100 * MILLISECONDS) {
            int action = uint32Rand() % 4;
            switch(action) {
                case 0:
                    if(s1Lock) break;
                    PRINTF("%c enter 1\n", id);
                    s1.enter();
                    s1Lock = true;
                    break;

                case 1:
                    if(s2Lock) break;
                    PRINTF("%c enter 2\n", id);
                    s2.enter();
                    s2Lock = true;
                    break;

                case 2:
                    if(!s1Lock) break;
                    PRINTF("%c leave 1\n", id);
                    s1.leave();
                    s1Lock = false;
                    break;

                case 3:
                    if(!s2Lock) break;
                    PRINTF("%c leave 2\n", id);
                    s2.leave();
                    s2Lock = false;
                    break;
            }
        }
    }
};


Requester r1('A');
Requester r2('B');
Requester r3('C');
