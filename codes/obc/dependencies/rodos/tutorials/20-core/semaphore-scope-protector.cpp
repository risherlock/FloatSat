#include "rodos.h"

Semaphore sema;

int myFunction() {
    PROTECT_IN_SCOPE(sema); // this macro creates a ScopeProtector on the stack here
                         // the semaphore is entered immediately
    static int cnt = 0;
    ++cnt;

    if(cnt % 10 == 0) return 1; // note, that we do not need to leave the semaphore manually
    if(cnt % 3 == 0) return 2;  // the semaphore is left within the destructor of the SemaphoreProtector

    return 3;
}

class TestThread : public StaticThread<> {
    int id;

  public:
    TestThread(const int id) : id(id) {}

    void run() {
        for(;;) {
            PRINTF("Thread %d: %d\n", id, myFunction());
        }
    }
};

TestThread t1(1), t2(2);
