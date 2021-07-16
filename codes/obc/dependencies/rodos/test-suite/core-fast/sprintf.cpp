#include "rodos.h"

uint32_t printfMask = 0;

class TestPrint : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;
        char outbuf[100];

        PRINTF(" prints from string: \n");

        SPRINTF(outbuf, "str = %d %d", 10, 20);

        PRINTF("  = '%s'\n-- DONE--\n", outbuf);
        hwResetAndReboot();
    }

} testPrint;
