

/**
* @file thread.cc
* @date 2008/04/22 16:50
* @author Lutz Dittrich, Sergio Montenegro
*
*
* A Thread is a schedulable object with own context and stack
*
* @brief %Thread handling
*/

#include "rodos.h"
#include "scheduler.h"
#include "hw_specific.h"
#include "platform-parameter.h"

namespace RODOS {

constexpr uint32_t EMPTY_MEMORY_MARKER = 0xDEADBEEF;

// at which time before the next thread becomes active, should the processor wake up?
constexpr int64_t TIME_BEFORE_WAKEUP = 3 * MILLISECONDS;
// if in the next "TIME_DONT_SLEEP"  nanoseconds the next thread becomes active, do sleep. 999 Seconds is as good as deactivated
constexpr int64_t TIME_DONT_SLEEP = 999*SECONDS;

//List Thread::threadList = 0;
//Thread* Thread::currentThread = 0;

/** old style constructor */
Thread::Thread(const char* name,
               const int32_t priority,
               const size_t _stackSize) :
    ListElement(threadList, name) {

    this->stackSize = _stackSize;
    stackBegin = static_cast<char*>(xmalloc(stackSize));
    stack = reinterpret_cast<long*>((reinterpret_cast<uintptr_t>(stackBegin) + (stackSize-4)) & (~static_cast<uintptr_t>(7u))); // align 8 byte
    this->priority = priority;

    //Paint the stack space; TODO: Comment out for faster start up
    initializeStack();
}

void Thread::initializeStack() {
    //Paint the stack space TODO: Comment out for faster start up
    uint32_t* stackPaint = (uint32_t*)stack;
    while((intptr_t)stackPaint >= (intptr_t)stackBegin) {
        *stackPaint = EMPTY_MEMORY_MARKER;
        stackPaint--;
    }

    context = hwInitContext(stack, this);
}

Thread::~Thread() {
    if(isShuttingDown) return;
    PRINTF("%s:",getName());
    RODOS_ERROR("Thread deleted");
}

/* called in main() after all constuctors, to create/init thread */
void Thread::create() {
    // only required when implementig in on the top of posix, rtems, freertos, etc
}

extern bool isSchedulingEnabled; // from scheduler

/** pause execution of this thread and call scheduler */
void Thread::yield() {
    if(!isSchedulingEnabled) return; // I really do not like This! but required

    /** Optimisation: Avoid unnecesary context swtichs: see Scheduler::schedule()  ***/
    long long timeNow = NOW(); 
    Thread* preselection = findNextToRun(timeNow); 
    if(preselection == getCurrentThread()) return;

    // schedule is required, The scheduler shall not repeate my computations: 
    Scheduler::preSelectedNextToRun = preselection; 
    Scheduler::preSelectedTime = timeNow;

    /* reschedule next timer interrupt to avoid interruptions of while switching */
    Timer::stop();
    __asmSaveContextAndCallScheduler();
}

/* restore context of this thread and continue execution of this thread */
void Thread::activate() {
    currentThread = this;
    Timer::setInterval(PARAM_TIMER_INTERVAL);
    Timer::start();
    __asmSwitchToContext((long*)context);
}


/*******************************************************************/

/* get priority of the thread */
int32_t Thread::getPriority() const {
    return priority;
}

/* set priority of the thread */
void Thread::setPriority(const int32_t prio) {
    priority = prio;
}

Thread* Thread::getCurrentThread() {
    return currentThread;
}


long long timeToTryAgainToSchedule = 0; // set when looking for the next to execute

/* resume the thread */
void Thread::resume() {
    timeToTryAgainToSchedule = 0;
    waitingFor     = 0;
    suspendedUntil = 0;
    // yield(); // commented out because resume may be called from an interrupt server
}

/* suspend the thread */
bool Thread::suspendCallerUntil(const int64_t reactivationTime, void* signaler) {

    Thread* caller =  getCurrentThread();
    {
        PRIORITY_CEILER_IN_SCOPE();
        caller->waitingFor = signaler;
        caller->suspendedUntil = reactivationTime;
    }
    yield();

    caller->waitingFor = 0;
    /** after yield: It was resumed (suspendedUntil set to 0) or time was reached ?*/
    if(caller->suspendedUntil == 0) return true; // it was resumed!
    return false; // time was reached
}



void Thread::initializeThreads() {
    xprintf("Threads in System:");
    ITERATE_LIST(Thread, threadList) {
        xprintf("\n   Prio = %7ld Stack = %6lu %s: ",
            static_cast<long>(iter->priority),
            static_cast<unsigned long>(iter->stackSize),
            iter->getName());
        iter->init();
        iter->suspendedUntil = 0;
    }
    xprintf("\n");
    ITERATE_LIST(Thread, threadList) {
        iter->create();
    }
}

// not used in this implementation, the scheduler activates thread
void Thread::startAllThreads() { }


/** non-static C++ member functions cannot be used like normal
   C function pointers. www.function-pointer.org suggests using a
   wrapper function instead. */

void threadStartupWrapper(Thread* thread) {
    Thread::currentThread = thread;
    thread->suspendedUntil = 0;

    thread->run();
    /*
      loop forever
      if run() returns this thread is to be considered terminated
    */

    while(1) {
        thread->suspendedUntil = END_OF_TIME;
        thread->yield();
    }
}


unsigned long long Thread::getScheduleCounter() {
    return Scheduler::getScheduleCounter();
}

/********************************************************************************/


/**
 * @class IdleThread
 * @brief The idle thread.
 *
 * The idle thread. This thread will be executed if no other thread wants to
 * run
 */
class IdleThread : public StaticThread<> {
public:
    IdleThread() : StaticThread("IdleThread", 0) {
    }
    void run();
    void init();
};

void IdleThread::run() {
    while(1) {
        idleCnt++;
        setPriority(0); // Due to wrong usage of PRIORITY_CLING in events, once I got highest prio for Idle.
        sp_partition_yield(); // allow other linux processes or ARIC-653 paritions to run
        yield();

        // enter sleep mode if suitable
        int64_t timerInterval = (timeToTryAgainToSchedule - NOW());
        timerInterval -= TIME_BEFORE_WAKEUP;
        if(timerInterval > TIME_DONT_SLEEP) {
          Timer::stop();
          Timer::setInterval(timerInterval / 1000l); // nanoseconds to microsenconds
          Timer::start();
          enterSleepMode();
          Timer::setInterval(PARAM_TIMER_INTERVAL);
          Timer::start();
        }
    }
}

void IdleThread::init() {
    xprintf("yields all the time");
}


/**
 * The idle thread.
 */
IdleThread idlethread;
Thread* idlethreadP = &idlethread;


/*********************************************************************************/

constexpr int64_t earlier(const int64_t a, const int64_t b) {
    return (a < b) ? a : b;
}

Thread* Thread::findNextToRun(int64_t timeNow) {
    Thread* nextThreadToRun = &idlethread; // Default, if no one else wants
    timeToTryAgainToSchedule = timeNow + TIME_SLICE_FOR_SAME_PRIORITY;
    ITERATE_LIST(Thread, threadList) {
        if (iter->suspendedUntil < timeNow) { // in the past
			// - thread with highest prio will be executed immediately when this scheduler-call ends
            // - other threads with lower prio will be executed after next scheduler-call
            //   due to suspend() of high-prio thread
            if (iter->getPriority() >  nextThreadToRun->getPriority()) { nextThreadToRun = iter; }
            if (iter->getPriority() == nextThreadToRun->getPriority()) {
                if (iter->lastActivation < nextThreadToRun->lastActivation) nextThreadToRun = iter;
            }

        } else { // in the future, find next to be handled
			// if there is a thread with higher or same priority in the future, we must call the scheduler then
			// so that the thread will be executed
            if(iter->getPriority() >= nextThreadToRun->getPriority()) { 
                timeToTryAgainToSchedule = earlier(timeToTryAgainToSchedule, iter->suspendedUntil) ;
            }
			// threads with lower priority will not be executed until nextThreadToRun suspends
        }
    } // Iterate list

    /** Check stack violations **/
    constexpr size_t stackMargin = 300;
    uintptr_t minimumStackAddr = reinterpret_cast<uintptr_t>(nextThreadToRun->stackBegin)+stackMargin;
    if(nextThreadToRun->getCurrentStackAddr() < minimumStackAddr) {
        xprintf("!StackOverflow! %s DEACTIVATED!: free %d\n",
            nextThreadToRun->name,
            static_cast<int>(nextThreadToRun->getCurrentStackAddr() - reinterpret_cast<uintptr_t>(nextThreadToRun->stackBegin)));
        nextThreadToRun->suspendedUntil = END_OF_TIME;
        nextThreadToRun = &idlethread;
    }
    if ( *reinterpret_cast<uint32_t *>(nextThreadToRun->stackBegin) !=  EMPTY_MEMORY_MARKER) { // this thread is going beyond its stack!
        xprintf("! PANIC %s beyond stack, DEACTIVATED!\n", nextThreadToRun->name);
        nextThreadToRun->suspendedUntil = END_OF_TIME;
        nextThreadToRun = &idlethread;
    }

    return nextThreadToRun;
}
#undef EARLIER


Thread* Thread::findNextWaitingFor(void* signaler) {
    Thread* nextWaiter = &idlethread; // Default, if no one else wants to run

    ITERATE_LIST(Thread, threadList) {
        if (iter->waitingFor == signaler) {
            if (iter->getPriority() > nextWaiter->getPriority()) {
                nextWaiter = iter;
            } else {
                if (iter->getPriority() == nextWaiter->getPriority()) {
                    if (iter->lastActivation < nextWaiter->lastActivation) {
                        nextWaiter = iter;
                    }
                }
            }
        }
    }
    if (nextWaiter == &idlethread) {
        return 0;
    }
    return nextWaiter;
}

size_t Thread::getMaxStackUsage(){
	Thread* currentThread = getCurrentThread();

	//Go to the beginning of the stack(lowest addres)
	uint32_t* stackScan = (uint32_t*)currentThread->stack;
	while((intptr_t)stackScan >= (intptr_t)currentThread->stackBegin){
		stackScan--;
	}
	stackScan++;

	//Go up until empty markers are found and count
	size_t freeStack=0;
	while(stackScan <= (uint32_t*)currentThread->stack && *stackScan == EMPTY_MEMORY_MARKER){
		freeStack +=4;
		stackScan++;
	}

	return currentThread->stackSize-freeStack;
}



} // namespace

