
#include "rodos.h"

#define LOW_STACK_LIMIT 300

/** Checks percent usage of each stack **/

namespace RODOS { // may not be in a nams spcae

class ThreadChecker : public StaticThread<> {
  public:
    ThreadChecker() : StaticThread<>("ThreadChecker") {}

    void init() { xprintf(SCREEN_RED "NOT TO FLY!!!!! checks stack of all threads " SCREEN_RESET); }

    void run() {
        PRINTF(" \n\n\n****************************************** IMPORTANT \n");
        PRINTF(" ****** to use this, set EMPTY_MEMORY_MARKER in thread_on_hw.cpp to 0 and recompile rodos\n");
        PRINTF(" ******************************************\n");
        size_t    minStack        = DEFAULT_STACKSIZE;
        Thread* dangerousThread = 0;
        TIME_LOOP(0, 3 * SECONDS) {
            PRINTF("TST: Threads and stacks:\n");
            ITERATE_LIST(Thread, Thread::threadList) {
                size_t  index = 0;
                char* stk   = iter->stackBegin;
                for(index = 16; index < iter->stackSize; index++)
                    if(stk[index] != 0) break;
                if(index < minStack) minStack = index;
                if(index < LOW_STACK_LIMIT) dangerousThread = iter;

                PRINTF("	%s  Prio = %7ld Stack = %6lu StackFree = %6lu, (min = %6lu)\n",
                       iter->getName(),
                       static_cast<long>(iter->priority),
                       static_cast<unsigned long>(iter->stackSize),
                       static_cast<unsigned long>(index),
                       static_cast<unsigned long>(minStack));
            }
            if(minStack < LOW_STACK_LIMIT) {
                PRINTF("TST: Danger! Stack to low\n");
                PRINTF("\nTST: Recommend to suspend '%s'****\n\n", dangerousThread->getName());
            }
        }
    }
} threadChecker;


/**************************************************/
class Innocent1 : public StaticThread<> {
  public:
    Innocent1() : StaticThread<>("Innocent1") {}
    void run() {}
} innocent1;
class Innocent2 : public StaticThread<> {
  public:
    Innocent2() : StaticThread<>("Innocent2") {}
    void run() {}
} innocent2;
class Innocent3 : public StaticThread<> {
  public:
    Innocent3() : StaticThread<>("Innocent3") {}
    void run() {}
} innocent3;


/*************************/


/*********** Other Threads to test stack occupation ***/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvla-extension"
void stackUser(int len) {
    char variableOnStack[len];
    for(int i = 0; i < len; i++) variableOnStack[i] = 0x5a;
}
#pragma GCC diagnostic pop


/** Consumes more an more stack until it crases *****/

class StackConsumer : public StaticThread<2000> {
  public:
    StackConsumer() : StaticThread<2000>("StackConsumer", 400) {}
    void run() {
        long consumer = 1000;
        TIME_LOOP(6 * SECONDS, 1 * SECONDS) {
            stackUser(consumer);
            consumer += 100;
            PRINTF("TST: StackConsumer using %ld stack bytes\n", consumer);
        }
    }
} stackConsumer;

} // namespace RODOS
