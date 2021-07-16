#include "rodos.h"

uint32_t printfMask = 0;

class TestSNPrintf: public StaticThread<> {
public:
  void run(){
    printfMask = 1;

    char buffer[32] = {};

    PRINTF(" Using SNPRINTF to protect from out of bounds memory access:\n\n");

    PRINTF(" For this test let's assume we only have 4 Bytes of Buffer\n");
    PRINTF(" We will print \"Hello World\" in there.\n");
    PRINTF(" If we only use our memory, we will only \"Hel\" and the trailing \\0\n\n");

    PRINTF(" Using SPRINTF we get: ");
    SPRINTF(buffer,"%s", "Hello World!");
    PRINTF("%s\n", buffer);
    PRINTF(" OOPS! We did use memory outside of this buffer!\n\n");

    PRINTF(" Using SNPRINTF we get: ");
    SNPRINTF(buffer, 4, "%s", "Hello World!");
    PRINTF("%s\n", buffer);
    PRINTF(" This time it worked!\n\n");

    PRINTF(" If you want to see what happens if the buffer is really that small, just change the buffer size.\n");
    PRINTF(" If you are lucky on your platform you get a segmentation fault, otherwise you just wrote memory that is not yours!\n");    
    hwResetAndReboot();
  
  }

} testPrint;



