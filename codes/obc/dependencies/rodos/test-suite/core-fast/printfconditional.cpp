#include "rodos.h"

// you will see no prints becouse printfMask = 0;
// set to 0xffffffff for all prints
// or do not define (comment out) to geht the default printfMask


uint32_t printfMask = 0; //Be aware: it is outside of namepsace RODOS


class TestPrint : public StaticThread<> {
  public:
    void run() {
        int loopCnt = 0;

        PRINTF_CONDITIONAL(0xffffffff, "\n even with printfMaks=0, with id all in 1 will allwas be printed\n");
        printfMask = 1;
        PRINTF("\nDid you see? no printf from main!************\n");
        PRINTF("My location in source code : %s\n", LOCATION);
        TIME_LOOP(100 * MILLISECONDS, 50 * MILLISECONDS) {
            PRINTF_CONDITIONAL(1, " mask/id 1\n");
            PRINTF_CONDITIONAL(2, " mask/id 2\n");
            PRINTF_CONDITIONAL(4, " mask/id 4\n");
            PRINTF_CONDITIONAL(8, " mask/id 8\n");
            PRINTF_CONDITIONAL(9, " mask/id 9 (bit 0 and 3 are set)\n");
            PRINTF_CONDITIONAL(0xffffffff, " ------- mask/id all in 1\n");
            printfMask <<= 1;
            loopCnt++;
            
            if (loopCnt > 10) {
                hwResetAndReboot();
            }
        }
    }

} testPrint;
