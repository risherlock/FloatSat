
/** READ THIS!!!! **/
/** To compute this value please use tutorials/development-tests/cpuspeedtest.cpp **/

#include <rodos.h>

namespace RODOS {

bool getIsHostBigEndian() { return isHostBigEndian; }

/* see .../development-tests/cspuspeedtest.cpp
 * !!! speed value depends on compiler optimization !!!
 * !!! this speedtest was performed with the restrictions of 'enable_uart=1' !!!
 */
int32_t        getSpeedKiloLoopsPerSecond() { return 66640; } // see .../development-tests/cspuspeedtest.cpp
int32_t        getMaxIdleCntPerSecond() { return 666400; }    // see rodos-tutorials/development-tests/cpuspeedtest.cpp
int32_t        getYieldTimeOverhead() { return 2200; }        // see tutorials/core/yieldtime.cpp
const char* getHostCpuArch() { return "raspberrypi3"; }
const char* getHostBasisOS() { return "baremetal"; }

} /* namespace RODOS */
