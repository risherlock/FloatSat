#include "rodos.h"

uint32_t printfMask = 0;

static Application module01("Testgetchar");

class TestGets : public StaticThread<> {
  public:
    TestGets() : StaticThread<>("testgetchar") {}
    void run() {
        printfMask = 1;
        int cnt = 0;
        char* s;
        PRINTF("Please type string of characters\n");
        TIME_LOOP(10 * MILLISECONDS, 1 * SECONDS) {
            cnt++;
            if (cnt > 5) {
                PRINTF("-------------------------------------------------------------");
                hwResetAndReboot();
            }
            if((s = getsNoWait()) != 0) { PRINTF("\n\n********* String: '%s' **********\n", s); }
            PRINTF("Waiting 1 Seconds\n");
        }
    }
} testGets;
