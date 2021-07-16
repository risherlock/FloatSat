#include "rodos.h"

uint32_t printfMask = 0;

class TestPrint : public StaticThread<> {
  public:
    void run() {
        printfMask = 1;

        PRINTF(" Showing differnt printf formats\n\n\n");

        double  a = 123456.789;
        double  b = -0.00000000123456789;
        double c;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
        *(int64_t*)&c = 0x7FF8000000000000; // nan
#pragma GCC diagnostic pop

        PRINTF(" as f : %f     %3.15f    %f\n", a, b, c);
        PRINTF(" as e : %e     %2.5e     %e\n", a, b, c);

        PRINTF("\n\n");
        PRINTF("as d:  %d\n", -12);
        PRINTF("as u:  %u\n", -12);
        PRINTF("as o:  %o\n", 12);
        PRINTF("as x:  %04x\n", 12);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
        PRINTF("as b:  %08b\n", 0x55); // please ignore the c++ compile warning
#pragma GCC diagnostic pop

        PRINTF("\n\n");
        PRINTF("as s: '%s'\n", "this is a string");
        
        hwResetAndReboot();
    }

} testPrint;
