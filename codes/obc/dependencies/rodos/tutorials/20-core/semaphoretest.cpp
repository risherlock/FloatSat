#include "rodos.h"

static Application module("semaphoretest");

Semaphore protector;

class TestThread : public StaticThread<> {
    void run() {
        while(1) {
            xprintf("%s\n", getName());
            yield();
            xprintf("%s before protector \n", getName());
            protector.enter();

            xprintf("------- %s entered at %3.9f \n", getName(), SECONDS_NOW());
            yield();
            xprintf("          %s 1.yield. getScheduleCounter()  %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("          %s 2.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("          %s 3.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("          %s 4.yield. getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            yield();
            xprintf("-------- %s leavs getScheduleCounter() %lld\n", getName(), getScheduleCounter());
            protector.leave();


            xprintf("%s after protector getScheduleCounter()  %lld \n", getName(), getScheduleCounter());
            yield();
        }
    }

  public:
    TestThread(const char* name) : StaticThread<>(name) {}
};


TestThread t1("aaa");
TestThread t2("bbb");
TestThread t3("ccc");
TestThread t4("ddd");
