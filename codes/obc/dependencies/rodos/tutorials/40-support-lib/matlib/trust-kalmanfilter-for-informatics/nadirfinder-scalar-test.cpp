#include "rodos.h"
#include "signalprocessing.h"




#include "rawsensors.h"


/**
Pre-prototype for nadirFinder

WARNING !!!!!   WARNING!!!! WARNING!!!!!
This is only to test the noise sensivity using only scalar operations

again: In this version there is NO rotation computations we just test
noise susceptibility!
*/


/**
nadirFinder: (komming later)
Using angles and rotation axis (including quaternions which uses angles)
to determine
 attitude, it is difficult to combine several measurements,
sensors and models:
eg combine (average) -179.999 and 179.999 degrees,
or rotation 180 degrees around (0,0,1) and around (0,0,-1)

Therefore to compute the attitude we find a vector which points from
the structure coordinates to the earth nadir. This provides only
information about roll and pitch and no heading. But for us ok.

Having nadir vectors we have no singularities or problems when
combining (average, weighted average)  different vectors coming
from different measurements, sensors and modes.


Important data:
   zVector: orotogonal to multicopter plate
   nadirVector: vector in the structure reference frame, points to nadir

   whenn we are horizontal zVector = nadirVector = (0,0,1)

**/


/***************** application dependent modell description *****/


static const double deltaT = 0.005; // 200 Hz

struct Stumuli {
    double gyro; // too dificult to compute roationsspeed from 6x motor thrust, we use gyro data instead!
};

struct State {
    TrustedVal nadirVector;
};

struct Sensors {
    TrustedVal accelerometer; // we normalize in the lowpass filter, here we use scale = 1
    TrustedVal magnetometer;
    // gyro is a sensor, yes, yes, but we use as it were stimuly to command rotations!
};


// Keeps same certainty and step bevor
State step(const State& pre, const Stumuli& u) {
    State next = pre; // copy the certainty -> same certainty a step bevor
    next.nadirVector.val = pre.nadirVector.val + u.gyro * deltaT;
    return next;
}

// certainty = combination of accelerometer and magenetometer
State senseState(const Sensors& z) {
    State sensed;
    TrustedVal measurement = z.accelerometer; // copy val and certainty
    measurement.mergeValAndCertainty(z.magnetometer);
    sensed.nadirVector = measurement; // copy both value and certainty
    return sensed;
}


State   modell;
State   sensed;
Sensors sensors;
Stumuli stimuli;

RawSensors rawSensors;
LowpassFilterObject<Vector3D> aFilter, gFilter, mFilter;

/** Following values are the magic in the solution **/
void initModell() {

    aFilter.historyLen = 4; // 4 optimal for 200 hz sample, 50 hz vibration.
    gFilter.historyLen = 4; // experimental: 4 reduces noise factor 5. and 8 reduces factor 8
    mFilter.historyLen = 4; // experimental: 4 has delay of one cyle, 8 has delay of 3 clycles
    aFilter.scale      = 2.0;    // unit g, max 2 g
    gFilter.scale      = 2*M_PI; // unit rad/sec, for us 1 == one rotation per second, we get radians

    modell.nadirVector.val          = 0;
    modell.nadirVector.certainty    = 0.95; // biger values: less noise but slower reactions

    sensors.accelerometer.certainty = 0.1; // shall be 0.005: noise of accelerometer = 1500x noise of magenetometer
    sensors.magnetometer.certainty  = 0.5; //   but magnotometer varies (no noise detected) from ship, motor-current etc
}



/***********************************************************/

void executeIt() {

    for(int stepCnt = 0; rawSensors.read(); stepCnt++) { // Be aware!!! here we read meassurmentes

        aFilter.accumulateNormalizing(rawSensors.acc);
        gFilter.accumulateNormalizing(rawSensors.gyro);
        mFilter.accumulateNormalizing(rawSensors.mag);

        stimuli.gyro              = gFilter.mean.y; // y hast the biggest noise
        sensors.accelerometer.val = aFilter.mean.z; // z points to nadir
        sensors.magnetometer.val  = mFilter.mean.z; // z has a nice jump

        /*** Estitame using model and sensors ***/
        modell = step(modell, stimuli);
        sensed = senseState(sensors);

        modell.nadirVector.mergeVal(sensed.nadirVector);

        PRINTF(" %f %f %f %f \n",
               modell.nadirVector.val,
               stimuli.gyro,
               sensors.accelerometer.val,
               sensors.magnetometer.val);
    }
}


/***********************************************************/



int printfVerbosity = 0;  // Avoid printfs from RODOS Start

void MAIN() {

    printfVerbosity = 100;  

    PRINTF("all = [\n");

    initModell();
    executeIt();

    PRINTF("];\n\n");
    PRINTF("n = all(:, 1);\n");
    PRINTF("g = all(:, 2);\n");
    PRINTF("a = all(:, 3);\n");
    PRINTF("m = all(:, 4);\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(n,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(g,  '-b', 'LineWidth',1);\n");
    PRINTF("plot(a,  '-g', 'LineWidth',1);\n");
    PRINTF("plot(m,  '-m', 'LineWidth',1);\n");
    PRINTF("legend('nadir', 'gyro', 'accelerometer', 'magnetometer');\n");

    PRINTF("input cr_to_end;\n");

    printfVerbosity = 0;  

}

