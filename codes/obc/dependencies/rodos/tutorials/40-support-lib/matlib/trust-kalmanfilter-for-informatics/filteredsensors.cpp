
#include "rodos.h"
#include "matlib.h"
#include "signalprocessing.h"

#include "rawsensors.h"
#include "filteredsensors.h"

void FilteredSensors::calibrateSensors(int numOfSteps) {
// Step 1:
    // Define scale factor
    aFilter.scale      = 2.0;    // unit g, max 2 g
    gFilter.scale      = 2*M_PI; // unit rad/sec, for us 1 == one rotation per second, we get radians
    mFilter.scale      = 1.0;

// Step 2:
    // First fast aproximation
    aFilter.historyLen = 10; // Short history to reach fast a good begin posisition
    gFilter.historyLen = 10;
    mFilter.historyLen = 10;
    for(int stepCnt = 0; stepCnt < numOfSteps; stepCnt++) { // for the first 500 meassurements we are in repose (see meassurement.m)
        rawSensors.read();
        aFilter.accumulateNormalizing(rawSensors.acc);
        gFilter.accumulateNormalizing(rawSensors.gyro);
        mFilter.accumulateNormalizing(rawSensors.mag);
    }
    rawSensors.close(); // reset and start again

// Step 3:
    // now slow adaption/reaction but very smooth calibration
    aFilter.historyLen = numOfSteps/2; // long history to reduce noise make a smooth  calibration
    gFilter.historyLen = numOfSteps/2;
    mFilter.historyLen = numOfSteps/2;
    for(int stepCnt = 0; stepCnt < numOfSteps; stepCnt++) { // for the first 500 meassurements we are in repose (see meassurement.m)
        rawSensors.read();
        aFilter.accumulateNormalizing(rawSensors.acc);
        gFilter.accumulateNormalizing(rawSensors.gyro);
        mFilter.accumulateNormalizing(rawSensors.mag);
    }
    rawSensors.close(); // reset and start again

// Step 4:
    // register offsets and initial values and rotors
    gyroOffset            = gFilter.getMean(); // around axis x
    nadirVectorInRepose   = aFilter.getMean().normalize();
    magnetVectorInRepose  = mFilter.getMean().normalize();

    // Rotor
    Rotor rot(magnetVectorInRepose, nadirVectorInRepose);
    magnetToNadirQ = Quaternion(rot);

// Step 5:
    // Now set the lowpassfilter for normal operation
    aFilter.historyLen = 4; // 4 optimal for 200 hz sample, 50 hz vibration.
    gFilter.historyLen = 16; // experimental: 4 reduces noise factor 5. and 8 reduces factor 8
    mFilter.historyLen = 4; // experimental: 4 has delay of one cyle, 8 has delay of 3 clycles
}

/***********************************************************/

bool FilteredSensors::readAndFilter() {
    bool ok = rawSensors.read();
    if(!ok) return false;

    rawSensors.gyro = rawSensors.gyro - gyroOffset;
    //rawSensors.acc  = rawSensors.acc  - nadirVectorInRepose;
    //rawSensors.mag  = rawSensors.mag  - magnetVectorInRepose;

    aFilter.accumulateNormalizing(rawSensors.acc);
    gFilter.accumulateNormalizing(rawSensors.gyro);
    mFilter.accumulateNormalizing(rawSensors.mag);
    return true;
}


