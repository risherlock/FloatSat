#include "rodos.h"

class HelloWorld : public Thread {
    void run() {
        PRINTF("Hello World!\n");
    }
} helloworld;
