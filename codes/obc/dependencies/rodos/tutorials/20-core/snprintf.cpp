#include "rodos.h"

class TestSNPrintf : public StaticThread<> {
  public:
    void run() {
        char buffer[32];

        PRINTF("assume 4 Bytes of Buffer\n");
        SPRINTF(buffer, "%s", "Hello World!");
        PRINTF("  using SPRINTF (...)  : '%s' (outside of buffer!)\n", buffer);
        SNPRINTF(buffer, 4, "%s", "Hello World!");
        PRINTF("  using SNPRINTF(..4..): '%s' (ok)\n", buffer);
    }

} testPrint;
