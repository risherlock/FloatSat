
/*
 * hostparameter.cpp
 * @author: Sergio Montenegro, Sept 2019
 *
 * Computes host performance parameter for a new port
 */


#include "rodos.h"

//_______________________________ compute  getYieldTimeOverhead and  getMaxIdleCntPerSecond

static const long NUM_OF_LOOPS  = 1000 * 1000 * 10;
volatile long long cnt = 0;

class SpeedTest : public StaticThread<> {
public:
    SpeedTest() : StaticThread<>("SpeedTest", 10) { }

    void run() {  // because of low priority, this will wait until yieldtest concludes

        /** how many loops in a second? **/
        int64_t startTime = NOW();
        for(cnt = 0; cnt < NUM_OF_LOOPS; cnt++);
        int64_t deltaTime = (NOW() - startTime);

        long kiloLoopsPerSecond = static_cast<long>((double)SECONDS * (double)NUM_OF_LOOPS / (double)deltaTime / 1000.0);
        PRINTF("long        getSpeedKiloLoopsPerSecond()  { return %ld; }\n", kiloLoopsPerSecond);

        int64_t myIdleCnt = idleCnt; // idleCnt from rodos scheduler
        suspendCallerUntil(NOW() + 10*SECONDS);
        int64_t idleDiff = idleCnt - myIdleCnt;
        PRINTF("long        getMaxIdleCntPerSecond()      { return %lld;}\n", static_cast<long long>(idleDiff/10));

        PRINTF("//_____________________________ DONE\n");

    }

} speedTest;



//_______________________________ compute getYieldTimeOverhead

#define NUMBER_OF_EXECUTIONS    1000000

Semaphore printProtect;

long long  yieldGlobal = 0;
class TestThread : public StaticThread<> {
  long long yieldCnt;
  void run(){

    PRINTF("\n\n________________________ Please wait until I say DONE\n");

    while(1){
      yield();
      yieldCnt++;
      yieldGlobal++;
      // displays every 100000 times
      if((yieldCnt%NUMBER_OF_EXECUTIONS) == 0){
        long long timeNow = NOW();
        PRINTF("long        getYieldTimeOverhead()        { return %ld; }\n", (long)( ((double)timeNow / (double)getScheduleCounter()) * NANOSECONDS ));
        AT(END_OF_TIME);    // Stops me and all other waiting for this semaphore
      }
    }	
  }
  //constructor
public:
  TestThread (const char* name ="xx") : StaticThread<>(name) { yieldCnt = 0; }

  void init() { PRINTF(" Thread activated"); }
};

/*****************************/
TestThread  a("A(1)"), b("B(2)"), c("C(3)");

 
