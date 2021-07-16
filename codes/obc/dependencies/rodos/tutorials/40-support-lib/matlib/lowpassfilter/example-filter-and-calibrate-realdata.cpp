
#include "rodos.h"
#include "signalprocessing.h"

#include "rawsensors.h"

int printfVerbosity = 0;  // Avoid printfs from RODOS Start

RawSensors rawSensors;
LowpassFilterObject<Vector3D> aFilter, gFilter, mFilter;


// Clibration and initial data
Vector3D nadirVectorInRepose;
Vector3D magnetVectorInRepose;
Vector3D gyroOffset;
//YPR      gyroOffset;
Quaternion magnetToNadirQ;

void calibrateSensors() {

    aFilter.historyLen = 10; // long history to reduce noise to calibrate
    gFilter.historyLen = 10;
    mFilter.historyLen = 10;

    aFilter.scale      = 2.0;    // unit g, max 2 g
    gFilter.scale      = 2*M_PI; // unit rad/sec, for us 1 == one rotation per second, we get radians
   
    for(int stepCnt = 0; stepCnt < 400; stepCnt++) { // for the first 500 meassurements we are in repose (see meassurement.m)
        rawSensors.read();
        aFilter.accumulateNormalizing(rawSensors.acc);
        gFilter.accumulateNormalizing(rawSensors.gyro);
        mFilter.accumulateNormalizing(rawSensors.mag);
    }
    rawSensors.close(); // reset and start again

    // now good filtered
    aFilter.historyLen = 200; // long history to reduce noise to calibrate
    gFilter.historyLen = 200;
    mFilter.historyLen = 200;

    for(int stepCnt = 0; stepCnt < 400; stepCnt++) { // for the first 500 meassurements we are in repose (see meassurement.m)
        rawSensors.read();
        aFilter.accumulateNormalizing(rawSensors.acc);
        gFilter.accumulateNormalizing(rawSensors.gyro);
        mFilter.accumulateNormalizing(rawSensors.mag);
        PRINTF(" %f %f %f ",  aFilter.mean.x, aFilter.mean.y, aFilter.mean.z);
        PRINTF(" %f %f %f ",  gFilter.mean.x, gFilter.mean.y, gFilter.mean.z);
        PRINTF(" %f %f %f\n", mFilter.mean.x, mFilter.mean.y, mFilter.mean.z);
    }
    rawSensors.close();

    gyroOffset           = gFilter.getMean();
    nadirVectorInRepose  = aFilter.getMean();
    magnetVectorInRepose = mFilter.getMean();

    double alpha = magnetVectorInRepose.getAngle(nadirVectorInRepose);
    Vector3D u   = magnetVectorInRepose.cross(nadirVectorInRepose);
    u = u.normalize();
    magnetToNadirQ = Quaternion(cos(alpha/2), u.scale(sin(alpha/2))); // this is already normalized!
}



void MAIN() { // RODOS Main
    printfVerbosity = 100;

    PRINTF("allx = [\n");
    calibrateSensors();
    PRINTF("];\n");

    aFilter.historyLen = 4; // 4 optimal for 200 hz sample, 50 hz vibration.
    gFilter.historyLen = 4; // experimental: 4 reduces noise factor 5. and 8 reduces factor 8
    mFilter.historyLen = 4; // experimental: 4 has delay of one cyle, 8 has delay of 3 clycles

    aFilter.scale      = 2.0;    // unit g, max 2 g
    gFilter.scale      = 2*M_PI; // unit rad/sec, for us 1 == one rotation per second, we get radians

    PRINTF("%% calibration values \n");
    PRINTF("%%  nadirVectorInRepose %f %f %f\n", nadirVectorInRepose.x, nadirVectorInRepose.y, nadirVectorInRepose.z);

    PRINTF("all = [;\n");
    while(rawSensors.read()) {
        rawSensors.acc  = rawSensors.acc  - nadirVectorInRepose;
        rawSensors.gyro = rawSensors.gyro - gyroOffset;
        rawSensors.mag  = rawSensors.mag  - magnetVectorInRepose;

        aFilter.accumulateNormalizing(rawSensors.acc);
        gFilter.accumulateNormalizing(rawSensors.gyro);
        mFilter.accumulateNormalizing(rawSensors.mag);

        PRINTF(" %f %f %f ",  aFilter.mean.x, aFilter.mean.y, aFilter.mean.z);
        PRINTF(" %f %f %f ",  gFilter.mean.x, gFilter.mean.y, gFilter.mean.z);
        PRINTF(" %f %f %f\n", mFilter.mean.x, mFilter.mean.y, mFilter.mean.z);
    }

    PRINTF("];\n\n");
    PRINTF("ax = all(:, 1);\n");
    PRINTF("ay = all(:, 2);\n");
    PRINTF("az = all(:, 3);\n");
    PRINTF("gx = all(:, 4);\n");
    PRINTF("gy = all(:, 5);\n");
    PRINTF("gz = all(:, 6);\n");
    PRINTF("mx = all(:, 7);\n");
    PRINTF("my = all(:, 8);\n");
    PRINTF("mz = all(:, 9);\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(ax,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(ay,  '-b', 'LineWidth',1);\n");
    PRINTF("plot(az,  '-g', 'LineWidth',1);\n");
    PRINTF("legend('ax', 'ay', 'az');\n");
    PRINTF("title 'accelerometer;'\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(gx,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(gy,  '-b', 'LineWidth',1);\n");
    PRINTF("plot(gz,  '-g', 'LineWidth',1);\n");
    PRINTF("legend('gx', 'gy', 'gz');\n");
    PRINTF("title 'Gyro;'\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(mx,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(my,  '-b', 'LineWidth',1);\n");
    PRINTF("plot(mz,  '-g', 'LineWidth',1);\n");
    PRINTF("legend('mx', 'my', 'mz');\n");
    PRINTF("title 'Magnetometer;'\n");

    PRINTF("input cr_to_end;\n");

    printfVerbosity = 0;  

}
