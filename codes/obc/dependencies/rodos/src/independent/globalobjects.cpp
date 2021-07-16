
/**
* @file 00globalobjects.cc
* @date 2009/04/22 16:50
* @author Sergio Montenegro
*
*
* Initialisation of global objects.
* normaly it shall be done in the corresponding class.cc
* but valgrind reports initialisation errors threfore they
* are in this file which will be linked first 
*
* @brief %Thread handling
*/


#include "initiator.h"
#include "listelement.h"
#include "subscriber.h"
#include "thread.h"
#include "timeevent.h"
#include "timepoints.h"


namespace RODOS {

/************************** HW dependet global varialbes **********/

int64_t timeAtStartup = 0LL;


/****************** src global variables **************/


Thread* Thread::currentThread   = 0;

List Thread::threadList         = 0;
List TimeEvent::timeEventList   = 0;
List Initiator::initiatorList   = 0;
List Subscriber::subscriberList = 0;


unsigned long rodosErrorCounter = 0;
const char*   rodosErrorMsg     = "all OK";

TimePoints<20>  errorLog;


} // namespace RODOS


