#include "rodos.h"
#include "signalprocessing.h"




#include "rawsensors.h"
#include "filteredsensors.h"



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
    Vector3D  gyro; // too dificult to compute roationsspeed from 6x motor thrust, we use gyro data instead!
};

struct State {
    TrustedObject<Vector3D> nadirVector;
};

struct Sensors {
    TrustedObject<Vector3D> accelerometer; // we normalize in the lowpass filter, here we use scale = 1
    TrustedObject<Vector3D> magnetometer;
    // gyro is a sensor, yes, yes, but we use as it were stimuly to command rotations!
};


FilteredSensors filteredSensors; 

// Keeps same certainty from step bevor
State step(const State& pre, const Stumuli& u) {
    State next = pre; // copy the certainty -> same certainty from step bevor
    next.nadirVector.val = pre.nadirVector.val.rpyRotate(u.gyro);
    return next;
}


// output certainty is a function of certainties of all used objects
State senseState(const Sensors& z) {

    State sensed; // value and certainty will come from magnetometer & accelerometer
    sensed.nadirVector     = z.magnetometer; // including certainty. Value is already normlized
    sensed.nadirVector.val = sensed.nadirVector.val.qRotate(filteredSensors.magnetToNadirQ);
    sensed.nadirVector.val = sensed.nadirVector.val.normalize(); // required?? or ist after rotation already normalized???
    
    sensed.nadirVector.mergeObjectAndCertainty(z.accelerometer); // both nadirVector and z.accelerometer are normalized
    sensed.nadirVector.val  = sensed.nadirVector.val.normalize();
    return sensed;
}

State           modell;
State           sensed;
Sensors         sensors;
Stumuli         stimuli;

LowpassFilterObject<Vector3D> nadirFiltered;
/** Following values are the magic in the solution ... and.... are not constant**/
void initModell() {

    modell.nadirVector.val          = Vector3D(0, 0, -1);
    modell.nadirVector.certainty    = 0.95; // values close to 1: less noise sensivity but slower reactions

    sensors.accelerometer.certainty = 0.05; // shall be 0.005: noise of accelerometer = 1500x noise of magenetometer
    sensors.magnetometer.certainty  = 0.50; //   but magnotometer varies (no noise detected) from ship, motor-current etc
                                           //     WARNING: magnetometer needs to be recalibrated as function of motor current
    nadirFiltered.historyLen = 16;
}


/***********************************************************/


void executeIt() {

    for(int64_t stepCnt = 0; true; stepCnt++) { 
        bool ok = filteredSensors.readAndFilter();
        if(!ok) break; // WARNING: end of file, in real fly there is no end!

        stimuli.gyro   = filteredSensors.gFilter.getMean(); 
        stimuli.gyro   = stimuli.gyro.scale(-deltaT); // WARNING: In this massurments gyro is inverted!!

        // WARNING: from here we use only normalized (-1 .. 1, or 0 ..1) values
        sensors.accelerometer.val  = filteredSensors.aFilter.mean; // Certainty as defined in initModell
        sensors.accelerometer.val  = sensors.accelerometer.val.normalize();
        sensors.magnetometer.val   = filteredSensors.mFilter.mean; // Certainty as defined in initModell
        sensors.magnetometer.val   = sensors.magnetometer.val.normalize();

        /*** Estitame using model and sensors ***/
        modell = step(modell, stimuli);
        sensed = senseState(sensors);
        modell.nadirVector.mergeObject(sensed.nadirVector);


        // ******** WARNING!!! If extra filter is required else remove
        {
            //nadirFiltered.accumulateNormalizing(modell.nadirVector.val);
            //modell.nadirVector.val = nadirFiltered.mean;
        }


        /*** only for graphics ****/
        {
            Vector3D zAxis(0, 0, -1);
            Rotor rot(modell.nadirVector.val, zAxis);
            RPY toprint(rot);

            PRINTF(" %f %f %f ", modell.nadirVector.val.x, modell.nadirVector.val.y, modell.nadirVector.val.z);
            PRINTF(" %f %f %f \n", toprint.x, toprint.y, toprint.z);
        }
    }
}


/***********************************************************/



int printfVerbosity = 0;  // Avoid printfs from RODOS Start

void MAIN() {
    printfVerbosity = 100;
    //PRINTF("sensors = [\n");
    filteredSensors.calibrateSensors(400); // Be aware! We read 2 time the same first 500 meassurementes
                                           //   by real fly, we read 2xN concecutive sensor data.
    //PRINTF("];\n\n");

    PRINTF("all = [\n");

    initModell();
    executeIt();

    PRINTF("];\n\n");
    PRINTF("x = all(:, 1);\n");
    PRINTF("y = all(:, 2);\n");
    PRINTF("z = all(:, 3);\n");
    PRINTF("ro= all(:, 4);\n");
    PRINTF("pi= all(:, 5);\n");
    PRINTF("ya= all(:, 6);\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(x,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(y,  '-g', 'LineWidth',1);\n");
    PRINTF("plot(z,  '-b', 'LineWidth',1);\n");
    PRINTF("legend('nadir-x', 'nadir-y', 'nadir-z');\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(ro,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(pi,  '-g', 'LineWidth',1);\n");
    PRINTF("plot(ya,  '-b', 'LineWidth',1);\n");
    PRINTF("legend('roll', 'pitch', 'yaw');\n");

    PRINTF("input cr_to_end;\n");

    printfVerbosity = 0;  
}

