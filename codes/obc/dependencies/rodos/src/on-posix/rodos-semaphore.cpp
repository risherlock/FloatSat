

/**
* @file semaphore.cc
* @date 2008/05/11 10:12
* @author Sergio Montenegro
*
* @brief %Semaphore - a synchronisation primitive
*
* binary semaphore, mutex
*
*/

#include <unistd.h>
#include <pthread.h>
#include <signal.h>
// #include <stdlib.h>


#include "rodos.h"

namespace RODOS {
pthread_mutex_t pthreadMutexInitialisation = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t globalAtomar = PTHREAD_MUTEX_INITIALIZER;

/*****************************/


/**
 *  Constructor
 */
Semaphore::Semaphore() :
  owner(0), ownerEnterCnt(0), ownerPriority(0)  {

	pthread_mutex_t *mutexp = new pthread_mutex_t;
	*mutexp = pthreadMutexInitialisation;
	context = (void*)mutexp;
}


/**
* caller will be blocked if semaphore is occupied
* Owner may reenter withput deadlock
*/
void Semaphore::enter() {
  Thread* caller = Thread::getCurrentThread();
  if(owner == caller) {
	ownerEnterCnt++;
	return;
  }
  pthread_mutex_t *mutexp = (pthread_mutex_t*)context;
  pthread_mutex_lock(mutexp);
  owner =  caller;
  ownerEnterCnt = 1;
}

/**
*  caller does not block. resumes one waiting trhead (enter)
*/
void Semaphore::leave() {
  Thread* caller = Thread::getCurrentThread();
  if (owner != caller) { // User Programm error: What to do? Nothing!
    return;
  }
  ownerEnterCnt--;
  if(ownerEnterCnt != 0) return;
  owner = 0;
  pthread_mutex_t *mutexp = (pthread_mutex_t*)context;
  pthread_mutex_unlock(mutexp);
  
}
}
