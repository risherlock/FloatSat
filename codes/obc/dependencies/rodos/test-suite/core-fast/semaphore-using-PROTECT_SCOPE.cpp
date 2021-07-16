#include "rodos.h"

#include "../prt-seconds-now.h"

uint32_t printfMask = 0;

static int prio = 1;        //priority of threads, priority is already used in Rodos

static Application module("semaphoretest");

Semaphore protector;

class TestThread : public StaticThread<> {
public:
    
    void run() {
        printfMask = 1;
        for (int i = 0; i < 10; i++) {
            PRINTF("%s\n", getName());
            yield();
            PRINTF("%s before protector \n", getName());

            {
                PROTECT_IN_SCOPE(protector);

                PRINTF("------- %s entered at %3.9f\n", getName(), CNT_SECONDS_NOW());
                yield();
                PRINTF("          %s 1.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                PRINTF("          %s 2.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                PRINTF("          %s 3.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                PRINTF("          %s 4.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
                yield();
                PRINTF("-------- %s leaves getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            }


            PRINTF("%s after protector getScheduleCounter()  %lld \n", getName(), getScheduleCounter());
            yield();
        }
        hwResetAndReboot();
    }

    TestThread(const char* name) : StaticThread<>(name, prio) {prio++;}
};


TestThread t1("aaa");
TestThread t2("bbb");
TestThread t3("ccc");
TestThread t4("ddd");
