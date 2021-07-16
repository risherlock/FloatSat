
#include "rodos.h"

#define LOW_STACK_LIMIT 300

/** Checks percent usage of each stack **/

/**************************************************/
class Innocent : public StaticThread<> {
  public:
    Innocent(const char* name) : StaticThread<>(name) {}
    void run() {
        TIME_LOOP(1 * SECONDS, 1 * SECONDS) { PRINTF("Innocent %s\n", name); }
    }
};

Innocent ino1("1");
Innocent ino2("2");
Innocent ino3("3");
Innocent ino4("4");

/*********** Other Threads to test stack occupation ***/

char dummyWriter;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winfinite-recursion"
void stackUser() {
    static int consumed = 0;
    char       variableOnStack[100];
    consumed += 100;
    for(int i = 0; i < 100; i++) variableOnStack[i] = 0x5a;
    dummyWriter = variableOnStack[10];
    PRINTF("Stackconsumer using %d\n", consumed);
    Thread::suspendCallerUntil(NOW() + 1 * SECONDS);
    stackUser();
}
#pragma GCC diagnostic pop


/** Consumes more an more stack until it crases *****/

class StackConsumer : public StaticThread<2000> {
  public:
    StackConsumer() : StaticThread<2000>("StackConsumer", 400) {}
    void run() { stackUser(); }
} stackConsumer;
