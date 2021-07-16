
/**
* @file thread.cc
* @date 2010/09/22 16:50
* @author Sergio Montenegro
*
*
* A Thread is a schedulable object with own context and stack
*
* @brief %Thread handling
*/

#include "rodos.h"
#include "scheduler.h"
#include "hw_specific.h"

#include <unistd.h>
#include <pthread.h>
#include <signal.h>
// #include <stdlib.h>
namespace RODOS {

pthread_mutex_t threadDataProtector = PTHREAD_MUTEX_INITIALIZER;

struct ThreadOnPosixContext{
	pthread_t pt;
	pthread_mutex_t mutex;
	pthread_cond_t condition;
};

/*********** dummy signal händler für all threads ***/
void threadSigtermHandler([[gnu::unused]] int sig) { } 

//List Thread::threadList = 0;
//Thread* Thread::currentThread = 0;

/** old style constructor */
Thread::Thread(const char* name,
               const int32_t priority,
               const size_t _stackSize) :
    ListElement(threadList, name) {

    this->stackSize = _stackSize;
    this->priority = priority;

    //Paint the stack space; TODO: Comment out for faster start up
    initializeStack();
}

Thread::~Thread() {
    if(isShuttingDown) return;
    PRINTF("%s:", getName());
    RODOS_ERROR("Thread deleted");
}

void Thread::initializeStack() {
    ThreadOnPosixContext* ctx =  new ThreadOnPosixContext;
    pthread_mutex_init(&ctx->mutex,0);
    pthread_cond_init(&ctx->condition,0);
    context = (long*) ctx;
}

/** it was better do make a wrapper for the wrapper becouse
 *  the threadStartupWrapper has some implementations in assembler
 *  and I did not dare to change them to be void* (*)(void*)
 */

pthread_mutex_t threadsGO = PTHREAD_MUTEX_INITIALIZER; // to wait until all threads are ready
void*           posixThreadEntryPoint(void *param) {
    pthread_mutex_lock(&threadsGO);
    pthread_mutex_unlock(&threadsGO);
    threadStartupWrapper((Thread*)param);
    return 0;
}

/* called in main() after all constuctors, to create/init thread */
void Thread::create() {

    /** Posix code **/
    static pthread_attr_t pthreadCreationAttr;
    static pthread_t      pt;

    pthread_attr_init(&pthreadCreationAttr);
    pthread_attr_setstacksize(&pthreadCreationAttr, stackSize);
    pthread_attr_setinheritsched(&pthreadCreationAttr, PTHREAD_EXPLICIT_SCHED);

    pthread_create(&pt, &pthreadCreationAttr, posixThreadEntryPoint, this);
    ((ThreadOnPosixContext*)context)->pt = pt;
    // xprintf("Thread %lx context %ld\n", (long)this, (long)context);

    setPriority(priority);
}


void checkSuspend(volatile int64_t* reactivationTime, pthread_cond_t* cond, pthread_mutex_t* mutex) {
    int64_t now                          = NOW();
    int64_t hostabsoluteReactivationTime = hwGetAbsoluteNanoseconds() + (*reactivationTime - now);

    struct timespec tp;
    tp.tv_sec  = static_cast<time_t>(hostabsoluteReactivationTime / SECONDS);
    tp.tv_nsec = static_cast<long>(hostabsoluteReactivationTime % SECONDS);

    while(*reactivationTime > now) {
        if(*reactivationTime == END_OF_TIME) {
            pthread_cond_wait(cond, mutex);
        } else {
            pthread_cond_timedwait(cond, mutex, &tp);
        }
        now = NOW();
    }
}


/** pause execution of this thread and call scheduler */
void Thread::yield() {
    //Make suspendUntil.. in genericIO work
    Thread*               caller =  getCurrentThread();
    ThreadOnPosixContext* context = (ThreadOnPosixContext*)(caller->context);

    pthread_mutex_lock(&context->mutex);
    if(caller->suspendedUntil > NOW()) {
        caller->waitingFor = 0;
        checkSuspend(&(caller->suspendedUntil), &context->condition, &context->mutex);
    }
    pthread_mutex_unlock(&context->mutex);


    sched_yield();
}

/* restore context of this thread and continue execution of this thread */
/** not used in posix?? */
// void Thread::activate() {
// currentThread = this;
// schedulerRunning = true;  /* a bit to early, but no later place possible */
// }


/*******************************************************************/

/* get priority of the thread */
int32_t Thread::getPriority() const {
    return priority;
}

/* set priority of the thread */
void Thread::setPriority(const int32_t prio) {
    //
    // posix priorities range from 1 to 99
    // rodos priorities range from 0 to 2G
    // normaly user uses from 0 to 1000, therfore convert best guess

    priority       = prio; // RODOS priority
    int32_t posixPrio = prio / 10;
    if(posixPrio < 1) posixPrio = 1;
    if(posixPrio > 99) posixPrio = 99;

    if(context == 0) return; // not initialized
    pthread_t pt = ((ThreadOnPosixContext*)context)->pt;

    /** Posix code **/
    // xprintf("Setting Prio %ld for %d\n", priority, (int)pt);
    struct sched_param param;
    memset(&param, '\0', sizeof(param));
    param.sched_priority = static_cast<int>(posixPrio);
    pthread_setschedparam(pt, SCHED_FIFO, &param);

    /*** Only debug ***/
    // int policy;
    // xprintf("Get scheduling parameters\n");
    // pthread_getschedparam(pt, &policy, &param);
    // xprintf("priority = %d ploicy = %d\n", param.sched_priority, policy);
    /************************/
}

Thread* Thread::getCurrentThread() {
    pthread_t posixCaller = pthread_self();

    Thread* me = 0;
    while(me == 0) {
        ITERATE_LIST(Thread, threadList) {
            pthread_t pthread = ((ThreadOnPosixContext*)iter->context)->pt;
            if(pthread == posixCaller) {
                me = iter;
                break;
            }
        }
        if(me == 0) {
            xprintf("Posix theads %ld not ready? !\n", (long)posixCaller);
            //yield();
            // sleep(1);
        }
    }
    return me;
}


/* resume the thread */
void Thread::resume() {
    ThreadOnPosixContext* c = (ThreadOnPosixContext*)context;
    pthread_mutex_lock(&c->mutex);
    suspendedUntil = 0;
    waitingFor     = 0;
    pthread_cond_signal(&c->condition);
    pthread_mutex_unlock(&c->mutex);
}


/* suspend the thread */
bool Thread::suspendCallerUntil(const int64_t reactivationTime, void* signaler) {

    Thread* caller =  getCurrentThread();
    ThreadOnPosixContext* context = (ThreadOnPosixContext*)(caller->context);

    pthread_mutex_lock(&context->mutex);

    caller->waitingFor = signaler;
    caller->suspendedUntil = reactivationTime;

    checkSuspend(&(caller->suspendedUntil), &context->condition, &context->mutex);

    pthread_mutex_unlock(&context->mutex);

    /** after yield: It was resumed (suspendedUntil set to 0) or time was reached ?*/
    if(caller->suspendedUntil == 0) return true; // it was resumed!
    caller->suspendedUntil = 0;
    return false; // time was reached
}


/******************************/

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
}

void Thread::startAllThreads() {

    pthread_mutex_lock(&threadsGO);

    ITERATE_LIST(Thread, threadList) {
        iter->create();
    }

    pthread_mutex_unlock(&threadsGO);
}

/** non-static C++ member functions cannot be used like normal
   C function pointers. www.function-pointer.org suggests using a
   wrapper function instead. */

void threadStartupWrapper(Thread* thread) {
    Thread::currentThread  = thread;
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


/*********************************************************************************/

// not used in posix: posix has its own scheluder
//Thread* Thread::findNextToRun(int64_t timeNow) { }

Thread* Thread::findNextWaitingFor(void* signaler) {
    Thread* nextWaiter = 0;

    ITERATE_LIST(Thread, threadList) {
        if (iter->waitingFor == signaler) {
	    if(nextWaiter == 0 ||
              (iter->getPriority() > nextWaiter->getPriority()) ) {
                nextWaiter = iter;
            }
        }
    }
    return nextWaiter;
}

size_t Thread::getMaxStackUsage() { return 0; }

} // namespace RODOS
