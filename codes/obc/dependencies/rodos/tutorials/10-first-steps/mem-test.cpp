#include "rodos.h"

// Just prints the address of a variable in global memory

uint8_t  var1 = 123;
uint32_t var2 = 456;
uint64_t var3 = 789;
uint64_t var4 = 000;

static Application application("MemTest");

class MemTest : public StaticThread<> {

  public:
    // Constructor
    MemTest() : StaticThread<>("mem-test") {}


    void init() {
        PRINTF("Mem Test: Just displaying mem locations of variables\n");
    }

    void run() {
        PRINTF("Mem Test: run\n");
        PRINTF("var1: %d@%lx\n", (int)var1, (long)&var1);
        PRINTF("var2: %d@%lx\n", (int)var2, (long)&var2);
        PRINTF("var3: %d@%lx\n", (int)var3, (long)&var3);
        PRINTF("var4: %d@%lx\n", (int)var4, (long)&var4);

        uint16_t cnt = 0;
        int64_t t0;
        while(1) {
            cnt++;
            suspendCallerUntil(NOW() + 2 * SECONDS);
            t0 = NOW();
            PRINTF("Time now: %lld - cnt: %d\n", static_cast<long long>(t0), cnt);
        }
    }
};

static MemTest memTest;
//static MemTest memTest2;
