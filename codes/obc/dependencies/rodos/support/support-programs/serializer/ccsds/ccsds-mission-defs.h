
#pragma once

/** if mission defines its own defs, it shall define the variable MISSION_DEFS **/

#ifndef MISSION_DEFS
#define MISSION_DEFS

// Recommendation: To be sure you use your own def and not this default
// Define in your mission_defs.h another (extra) variable and use it!
// OK! I define here MISSION_NAME

#define MISSION_NAME "MttM-1" // used only as info in printfs

namespace CCSDS {

static const int TF_MAX_LEN          = 1024;   ///< MAX Of down and up link
static const int SP_MAX_LEN          = 1000;   ///< MAX Of down and up link
static const int UPLINK_TF_MAX_LEN   = 1024;
static const int UPLINK_SP_MAX_LEN   = 1000;   ///< WARNING!! if you modify, modify "tcm-extended-telemetry.hh" too !!!!
static const int DOWNLINK_TF_LEN     = 1000;   ///< Downlink has a fixed len
static const int DOWNLINK_SP_MAX_LEN = DOWNLINK_TF_LEN -6 -4 -2;  // 6 bytes TF-header  4 bytes CLCW  2 Bytes CRC
static const int MAX_USER_DATA_LEN   = DOWNLINK_SP_MAX_LEN -16;   // 6 Primary SP-header, 10 Secondary SP-Header

//static const int SPACECRAFT_ID       = 123;
static const int CRC_SEED            = 0xffff;
//static const int CRC_SEED            = 0x0000; ///< not ccsds standard! used as authentification
//static const int CRC_SEED            = 12345; ///< not ccsds standard! used as authentification

} // namespace ccsds


#endif

