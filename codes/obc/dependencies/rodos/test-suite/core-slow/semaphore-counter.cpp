#include "rodos.h"

uint32_t printfMask = 0;


int64_t counter = 0;
static const int64_t COUNT_LIMIT = 1000 * 1000 * 100;

static const int NUM_OF_THREADS = 20;
int threadsFinisched = 0;

Semaphore protector;

class CountThread : public StaticThread<> {
public:
  CountThread() : StaticThread<>() { }
  void run() {
    printfMask = 1;
    protector.enter();
    for(int64_t i = 0; i < COUNT_LIMIT; i++) {
        counter++;
    }
    threadsFinisched++;
    protector.leave();
    if(threadsFinisched >=  NUM_OF_THREADS) {
        PRINTF(" %d from %d Threads finished -> terminate\n", threadsFinisched, NUM_OF_THREADS);
        PRINTF("counter at the end = %lld\n", static_cast<long long>(counter));
        hwResetAndReboot();
    }
  }
};


CountThread ct[NUM_OF_THREADS];


