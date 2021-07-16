
#pragma once
#include "stdint.h"
#include "ccsds-telemetry-headers.hh"


/** this is your normal telemetry: Not typical examples!!! ***/

using namespace Common;

struct LanderTelemetry {
    TranferFrameHeader  transferFrameHeader;
    SourcePacketHeader  sourcePacketHeader;

    double   descentSpeed;
    int64_t  timeNow;
    bool     legContact[4];

    TranferFrameTrailer tranferFrameTrailer;

    /****************************************/
    LanderTelemetry() { timeNow = 0; descentSpeed = 20.3; }
    void setLanded() { legContact[0] = legContact[1] = legContact[2] = legContact[3] = true; }

    int serialize(char * const) const;
    int deserialize(char const * const);

};


