
/** To compute this value please use tutorials/development-tests/cpuspeedtest.cpp **/

#include "rodos.h"

namespace RODOS {


int32_t  getSpeedKiloLoopsPerSecond()  { return 6995;  } // see tutorials/20-core/compute-hostinfos.cpp
int32_t  getMaxIdleCntPerSecond()      { return 70000; } // see tutorials/20-core/compute-hostinfos.cpp
int32_t  getYieldTimeOverhead()        { return 50000; } // see tutorials/20-core/compute-hostinfos.cpp

bool  getIsHostBigEndian()          { return isHostBigEndian; }  // Yes! Actually getIsHostBigEndian is host independent
const char* getHostCpuArch()        { return "template";    }
const char* getHostBasisOS()        { return "baremetal"; }
}
