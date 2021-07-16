



/**
* @file rodos-semaphore.h
* @date 2008/05/10 16:37
* @author Sergio Montenegro
*
*
* @breif binary semaphore, mutex a synchronisation primitive 
*/

#pragma once

#include "thread.h"

namespace RODOS {

/**
* @class Semaphore
* @brief a synchronisation primitive
*
* Mutex Semaphore.
* the next thread to be activated is
* selected according to priorities and
* not come first serve first.
*
*/

class Semaphore {

private:
  Thread* volatile owner;       ///< A pointer to the thread that currently has entered the semaphore.
  volatile int ownerEnterCnt;   ///< Counts how often the owner enters the semaphore.

protected:
  volatile int32_t ownerPriority;   ///< The scheduling priority of the thread that currently has entered the semaphore.
  void* context; ///< used only on posix and on host-os

public:
  /** Constructor */
  Semaphore();
  // ~Semaphore() { } // Shall never be called. Semaphores may not disappear

  /** caller will be blocked if semaphore is occupied
   ** The owner may reenter the semaphore without deadlock */
  void enter();

  /** caller does not block. Resumes one waiting thread (enter) */
  void leave();

  /** true if semaphore is free:
  *   Warning: next it can be occupied by someone else
  */
  // bool isFree();

  /// This is just a help for the macro PROTECT_WITH_SEMAPHORE (DEPRECATED)
  // inline bool enterRetTrue() { enter(); return true; }
};

// Use this class to protect a scope with a semaphore.
// Just create an instance on the stack; it immediately enters a given semaphore.
// On destruction it leaves the semaphore.
class ScopeProtector {
  Semaphore *sema;    ///< Pointer to the semaphore that is used for protection.
  bool left = false;  ///< Indicates whether the semaphore has already been left.

public:
  /**
   * The constructor immediately enters the passed semaphore.
   * @param inputSema The semaphore that shall be entered.
   */
  explicit ScopeProtector(Semaphore *const inputSema) : sema(inputSema) { sema->enter(); }

  /**
   * The destructor leaves the semaphore.
   */
  ~ScopeProtector() { leave(); }

  /*
   * If the semaphore should be left before destroying the protector object, invoke this method.
   */
  void leave() { if(!left) sema->leave(); left = true; }
};

/**
 * Macro that initializes a %ScopeProtector object on the stack.
 */
#define PROTECT_IN_SCOPE(_sema) ScopeProtector _scope_protector_(&(_sema));

#if 0
// DEPRECATED! use PROTECT_IN_SCOPE instead
// #define PROTECT_WITH_SEMAPHORE(sem) \  !!!Commented out: DEPRECATED!!!
// for(bool _onlyonce_ = sem.enterRetTrue(); _onlyonce_ ; _onlyonce_ = false, sem.leave())
#endif

} // namespace

