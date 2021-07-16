
/** READ THIS!!!! **/
/** To compute this value please use tutorials/development-tests/cpuspeedtest.cpp **/

#include "rodos.h"

namespace RODOS {

bool  getIsHostBigEndian()          { return isHostBigEndian; }

int32_t  getSpeedKiloLoopsPerSecond()  { return 6995;  } // see .../development-tests/cspuspeedtest.cpp
int32_t  getMaxIdleCntPerSecond()      { return 70000; } // see rodos-tutorials/development-tests/cpuspeedtest.cpp
int32_t  getYieldTimeOverhead()        { return 50000; } // see tutorials/core/yieldtime.cpp
const char* getHostCpuArch()        { return "cortex-M4 (STM32F4)";    }
const char* getHostBasisOS()        { return "baremetal"; }
}
