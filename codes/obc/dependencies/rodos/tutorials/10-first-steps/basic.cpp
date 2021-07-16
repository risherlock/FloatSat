#include "rodos.h"

static Application nameNotImportantHW("HelloWorld");

class HelloWorld : public StaticThread<> {

  public:
    HelloWorld() : StaticThread<>("HelloWorld") {}

    void init() {
        PRINTF(SCREEN_RED "This is init() for Printing Hello World" SCREEN_RESET);
    }

    void run() {
        PRINTF("Hello World!\ndid you see the init in red?\n");
    }
};

HelloWorld helloworld;
