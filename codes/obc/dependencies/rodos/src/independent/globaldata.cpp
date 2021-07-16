
/**
* @file globaldata.cc
* @date 2011/04/22 17:08
* @author Sergio Montenegro
*
* @brief Global data which shall be initialize by init
*
*/

#include "stdint.h"


#include "thread.h"


namespace RODOS {


/*********************************************/

bool isShuttingDown = false; // set by hwResetAndReboot(), read by destructors

/**
 * In case a network is avialable, the corresponding node nr.
 */

int32_t myNodeNr = -1 ;
int32_t getNodeNumber() { return myNodeNr; }

/**
 * In case there is a getways: number of arrived messages
 */

int64_t numberOfReceivedMsgsFromNetwork = 0;
int64_t getNumberOfReceivedMsgsFromNetwork() {
    int64_t local_numberOfReceivedMsgsFromNetwork = 0;

    PRIORITY_CEILER_IN_SCOPE();
    local_numberOfReceivedMsgsFromNetwork=numberOfReceivedMsgsFromNetwork;
    return local_numberOfReceivedMsgsFromNetwork;
}

volatile int64_t idleCnt = 0;

}  // namespace RODOS

