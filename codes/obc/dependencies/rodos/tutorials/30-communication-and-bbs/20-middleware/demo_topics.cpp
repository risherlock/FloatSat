
#include "rodos.h"
#include "demo_topics.h"
#include "mask-screen.h"

Topic<long>    counter1(-1, "counter1");
Topic<long>    counter2(-1, "counter2");
Topic<Position>  position(-1, "position");

void MAIN() {
    INIT_MASK()
    xprintf("Please screen at least 23x80 chars\n");
}

