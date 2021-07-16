#include "rodos.h"

uint32_t printfMask = 0;

class HelloWorld : public StaticThread<> {
    void run() { PRINTF("Hello World!\n");
        printfMask = 1;
        PRINTF("Hello World!\n");
        hwResetAndReboot();
    }
} helloworld;
