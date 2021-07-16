#include "rodos.h"


class TestPrint : public StaticThread<> {
  public:
    void run() {
        char outbuf[100];

        PRINTF(" prints from string: \n");

        SPRINTF(outbuf, "str = %d %d", 10, 20);

        PRINTF("  = '%s'\n-- DONE--\n", outbuf);
    }

} testPrint;
