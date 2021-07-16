
/**
* @file thread.h
* @date 2008/05/04 16:21
* @author Lutz Dittrich, Sergio Montenegro
*
* A Thread is an active schedulable object with own context and stack
*
* @brief %A %thread (thread)
*/

#pragma once

#include <stdint.h>

#include "listelement.h" // required when compilng with posix
#include "timemodel.h"
#include "default-platform-parameter.h"

#include <stddef.h>

namespace RODOS {

extern "C" {
  void schedulerWrapper(long* ctx);
}

long* hwInitContext(long* stack, void* thread);

/**
* @class Thread
* @brief %Thread provides stack and context management
* To implement a thread, users must inherit from a thread class and implement the run() method.
* The standard way is to inherit from %StaticThread, which uses a static allocated stack.
*/
class Thread : public ListElement {
  friend void schedulerWrapper(long* ctx);
  friend void threadStartupWrapper(Thread*);
  friend void initSystem();
  friend void startIdleThread();
  friend class Scheduler;
  friend class ThreadChecker; // not in RODOS, maybe created by users
  friend class GenericIOInterface;

private:
  static List threadList; ///< List of all threads
  size_t stackSize; 	  ///< size of the thread's stack in bytes
  long* stack; 		  ///< pointer to the thread's stack (beginning high, growing low)
  char* stackBegin;	  ///< stack grows down, this is the lower limit
  long* volatile context; ///< pointer to stored context

  // Activation control
  /**  priority of thread, higher values are serverd first  */
  volatile int32_t priority = 0;

  /** It will be activated only after this time */
  volatile int64_t suspendedUntil = 0;

  /** if waiting for reactivation from someone, eg semaphore */
  void* waitingFor = nullptr;

  int64_t nextBeat = END_OF_TIME;  ///<  the next time to awake (used in wait)
  int64_t period = 0;    ///<  To repeat every period localTime units

  void create(); ///< called in main() after all constuctors, to create/init thread

  // Used only by the Scheduler and ThreadManager (friends)
  volatile unsigned long long lastActivation = 0; ///< used by scheduling algorithm
  void activate(); ///< continue the execution of the thread
  void initializeStack();

  static void initializeThreads(); ///< call the init method of all threads
  static Thread* currentThread; ///< pointer to currently running thread

public:

  /**
   * Initialization of the thread. A user should use a meaningful name for the thread.
   *
   * @param name Name of the thread.
   * @param priority The priority for the thread. Only the thread with the highest value from
   *        several runnable threads at a time get computing resources.
   * @param stackSize The size of the stack memory for the thread. Choice a matching value for
   *        the task of the thread.
   *
   * @see DEFAULT_THREAD_PRIORITY
   * @see DEFAULT_STACKSIZE
   */
  [[deprecated("consider using StaticThread!")]]
  Thread(const char*   name      = "AnonymThread",
         const int32_t priority  = DEFAULT_THREAD_PRIORITY,
         const size_t  stackSize = DEFAULT_STACKSIZE);

  /**
   * Initialization of the thread. A user should use a meaningful name for the thread.
   *
   * @param stack Reference to the allocated space to be used as stack of this thread.
   *        This has to reference a char array of size STACK_SIZE!
   * @param name Name of the thread.
   * @param priority The priority for the thread. Only the thread with the highest value from
   *        several runnable threads at a time get computing resources.
   * 
   * @tparam STACK_SIZE The size of the stack memory for the thread.
   *
   * @see DEFAULT_THREAD_PRIORITY
   * @see DEFAULT_STACKSIZE
   */
  template <size_t STACK_SIZE>
  Thread(char (&stack)[STACK_SIZE],
         const char*   name     = "AnonymThread",
         const int32_t priority = DEFAULT_THREAD_PRIORITY)
    : ListElement(threadList, name) {
      this->stackSize  = STACK_SIZE;
      this->stackBegin = stack;
      this->stack      = reinterpret_cast<long*>(
        (reinterpret_cast<uintptr_t>(stackBegin) + (stackSize - 4)) & (~static_cast<uintptr_t>(7u)));
      this->priority   = priority;

      initializeStack();
  }

  /// Destructor
  virtual ~Thread();

  /**
   * Entry point for user code. The thread activities shall implement by overloading this method.
   */
  virtual void run() = 0;

  /**
   * The method is called after all constructors are executed and before the threads are activated
   * by a call to the method run.
   *
   * It should be overloaded in case the thread has to call other objects for the initialization,
   * hence after all instances are present and before any run method is invoked.
   *
   * @see run
   */
  virtual void init() {}

  /**
   * Get priority of thread.
   *
   * @return The priority of the thread.
   */
  int32_t getPriority() const;

  /**
   * Set priority of thread
   *
   * @param[in] prio The new priority of the thread.
   */
  void setPriority(const int32_t prio);

  static void startAllThreads(); ///< if required (only on gast OSs) called by scheduler

  /**
   * Suspend the thread until a given time or a wake up by a call to the method resume.
   * By the optional pointer to a signaler the thread could be woken up. A signal is for
   * example an object of type inherited from class Event.
   *
   * @param[in] reactivationTime Optional time to wake up. Default value is end of time.
   * @param[in] signaler Pointer to signaler, which can resume the thread.
   *
   * @return True if the thread is resumed, false if the reactivation time is reached.
   *
   * @see resume
   * @see Event
   */
  static bool suspendCallerUntil(const int64_t reactivationTime = END_OF_TIME, void* signaler = 0);

  /**
   * Suspend a thread until the next computing period of the thread start. The computing periods are
   * computed from a start and a periodical time set with the method setPeriodicBeat. The thread is
   * suspend for ever if setPeriodicBeat isn't called before a call to this method.
   *
   * A call to method resume by another thread or an subscribed event or topic can wake up the thread
   * before the next computing period starts. In this cases the reason shall checked by the thread after
   * wake up.
   *
   * @see setPeriodicBeat
   * @see resume
   * @see Subscriber
   */
  void suspendUntilNextBeat();

  /**
   * Defines the start of computing periods for the method suspendUntilNextBeat.
   *
   * @param begin Time for first the start time of the first computing period.
   * @param period Period between the start time of computing periods.
   *
   * @see suspendUntilNextBeat
   */
  void setPeriodicBeat(const int64_t begin, const int64_t period);

  /**
   * Resume the thread. The thread gets after the call to the method computing resources if no
   * other thread with a higher priority occupy the resources. It end calls to the methods
   * suspendCallerUntil and suspendUntilNextBeat.
   *
   * @see suspendCallerUntil
   * @see suspendUntilNextBeat
   * @see Semaphore
   */
  void resume();

   /**
    * Like resume, but if the resumed thread has a higher priority than the runing one
    * then a task switch will take place at once.
    *
    * WARNING: NEVER call it from an interrupt server
    */

    void resumeAndYield() { resume() ; yield(); }

  /**
   * Returns the current stack pointer of the thread.
   */
  uintptr_t getCurrentStackAddr();

  /**
   * Cause the currently running thread to pause and perform a reschedule.
   */
  static void yield();

  /**
   * Get a pointer to the currently running thread. The method can used to identify the calling thread in
   * classes with no inheritance to the class Thread.
   *
   * @return Pointer to the currently running thread.
   */
  static Thread* getCurrentThread();

  /**
   * Search over all threads and select the one with the highest priority which is ready to run
   *
   * @return The pointer to the highest priorized runnable thread.
   *
   * @see resume
   */
  static Thread* findNextToRun(int64_t timeNow);

  /**
   * Search over all threads and select the one with the highest priority which is not ready to run
   * and waiting for the given signaler.
   *
   * @param signaler Pointer to the signaler where the thread has to wait on.
   *
   * @return Pointer to the highest priorized thread waiting on the signaler.
   */
  static Thread* findNextWaitingFor(void* signaler);

  /**
   * Get the schedule counter.
   *
   * @return The current schedule counter. The counter is increased for each schedule operation.
   */
  static unsigned long long getScheduleCounter();

  /**
   * Sets the priority of the currently running thread. Use it carefully.
   *
   * @param newPrio Priority to set to the currently running thread.
   *
   * @return The previous priority of the currently running thread.
   *
   * @see setPriority
   */
  static int32_t setPrioCurrentRunner(int32_t newPrio);

  /**
   * Get the maximum stack usage of the current thread until now. May return 0 if not supported.
   * All mare-metal plattform supoort it.
   *
   * @return The maximum stack usage of the current thread until now.
   */
  static size_t getMaxStackUsage();

};


/**
 * This class is the standard way to use threads.
 * By inheriting from this class, developers can implement their own threads.
 * @tparam STACK_SIZE The size of the stack.
 */
template <size_t STACK_SIZE = DEFAULT_STACKSIZE>
class StaticThread : public Thread {
    char stack alignas(8)[STACK_SIZE];  ///< The stack as a member byte array.
public:
    /**
    * Constructor initializing the %Thread.
    * @param name The name of the thread.
    * @param priority The priority of the thread.
    */
    StaticThread(const char* name = "AnonymThread", const int32_t priority = DEFAULT_THREAD_PRIORITY)
      : Thread(stack, name, priority) {}
};

/******************************************************
 * Shortcuts for often used constructs
 *******************************************************/

/**
 * A pointer to the currently running thread.
 **/

inline RODOS::Thread* RUNNER() {
    return Thread::getCurrentThread();
}

/**
 * TIME_LOOP is a shortcut for setting and using time beats
 * TIME_LOOP(begin,period) both in localTime units
 * using setPeriodicBeat(...) and waitUntilNexBeat()
 */

#define TIME_LOOP(_begin, _period) for(setPeriodicBeat(_begin, _period);suspendUntilNextBeat(), true; )


/**
 * AT is a shortcut to wait until a time point.
 * use AT(time); ....
 * to suspend for always use
 * AT(END_OF_TIME);
 * @param time The time in internal Rodos (up)time.
 */
inline void AT(const int64_t time)     { Thread::suspendCallerUntil(time); }

/**
 * AT_UTC is a shortcut to wait until a time point, given in UTC.
 * @param timeInUTC The time in UTC
 */
inline void AT_UTC(const int64_t timeInUTC) { Thread::suspendCallerUntil(sysTime.UTC2LocalTime(timeInUTC)); }

/**
 * This function returns as soon as the passed time was reached.
 * The thread is not suspended explicitly, it keeps the CPU busy.
 * @param endWaitingTime
 */
inline void BUSY_WAITING_UNTIL(int64_t endWaitingTime) { while(NOW() < (endWaitingTime)) ; }


/**
 * Usually you can create short atomar sections using the macro PRIORITY_CEILER_IN_SCOPE
 * which is the standard, recommended procedure.
 * 
 * But some applications demand enabling and disabling the scheduling process. 
 * For those applications we use globalAtomarLock and globalAromarUnlock.
 *
 * WARNING!!!:   Deprecated
 *   avoid it! (no implementation on posix derivates)
 *   In multicore processors it does not help!
 */

extern bool globalAtomarLock();   ///< returns always true
extern bool globalAtomarUnlock(); ///< returns always true


/* Use this class to change Priority in a scope.
 * Just create an instance on the stack; it immediately changes priority.
 * On destruction it returns to the old priority.
 */
class ScopePriority {
  int32_t previousPriority; ///< The original priority that should be restored when leaving the given priority.

public:
  /**
   * The constructor taking the temporary  thread priority that should be used during the object's lifecycle.
   * @param newPriority The temporary thread priority that should be used during the object's lifecycle.x
   */
  explicit ScopePriority(int32_t newPriority) { previousPriority = Thread::setPrioCurrentRunner(newPriority); }

  /**
   * Destructor resetting the thread priority back to the original value.
   */
  ~ScopePriority()                         { Thread::setPrioCurrentRunner(previousPriority); }
};

/**
 * Macro that initializes a %ScopePriority object on the stack.
 */
#define PRIORITY_IN_SCOPE(_prio) ScopePriority   _scopePriority_(_prio)


/**
* Priority ceiling is used to avoid thread switches in a critical region.
* This is equivalent to atomarity at thread level.
* This has no effect to interrupts and is no interrupt atomarity
*/
#define PRIORITY_CEILER_IN_SCOPE()   ScopePriority _scopePriority_ (CEILING_PRIORITY)

/**
* Priority flooring is used busy waiting loop in order to
* reduce your priority to do not consume the time some one else would need
*/
#define PRIORITY_FLOORING_IN_SCOPE() ScopePriority _scopePriority_ (FLOORING_PRIORITY)


/** to see if the scheduler is active but please do not modify it! (yes, yes, later I will make a function of it)*/
extern volatile bool schedulerRunning;

}  // namespace

