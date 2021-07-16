/** READ THIS!!!! **/
/** To compute this value please use tutorials/development-tests/cpuspeedtest.cpp **/

#include "rodos.h"

namespace RODOS {

bool getIsHostBigEndian() { return isHostBigEndian; }

/* see .../development-tests/cspuspeedtest.cpp
 * !!! speed value depends on compiler optimization !!!
 *
 *   SoC  |  compiler | optimization | IdleCntPerSecond | KiloLoopsPerSecond
 * -------|-----------|--------------|------------------|-------------------
 * M2S060 |   4.9.3   |   -Og        |     31552        |       783
 *
 *
 */
int32_t        getSpeedKiloLoopsPerSecond() { return 783; } // see rodos-tutorials/development-tests/cpuspeedtest.cpp
int32_t        getMaxIdleCntPerSecond() { return 31552; }   // see rodos-tutorials/development-tests/cpuspeedtest.cpp
int32_t        getYieldTimeOverhead() { return 30000; }     // see tutorials/core/yieldtime.cpp
const char* getHostCpuArch() { return "arm cortex-m3"; }
const char* getHostBasisOS() { return "baremetal"; }

} /* namespace RODOS */
