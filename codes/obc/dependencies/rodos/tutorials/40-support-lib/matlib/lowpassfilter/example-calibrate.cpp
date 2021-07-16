
#include "rodos.h"
#include "signalprocessing.h"

#include "rawsensors.h"

int printfVerbosity = 0;  // Avoid printfs from RODOS Start


RawSensors rs;
LowpassFilterObject<Vector3D> aFilter, gFilter, mFilter;

void MAIN() { // RODOS Main
    printfVerbosity = 100;  
    aFilter.historyLen = 100; // 4 optimal for 200 hz sample, 50 hz vibration.
    gFilter.historyLen = 100; // experimental: 4 reduces noise factor 5. and 8 reduces factor 8
    mFilter.historyLen = 100; // experimental: 4 has delay of one cyle, 8 has delay of 3 clycles

    PRINTF("all = [;\n");
    
    for(int stepCnt = 0; stepCnt < 400; stepCnt++) {
        if(!rs.read()) break;
        aFilter.accumulateAsIs(rs.acc);
        gFilter.accumulateAsIs(rs.gyro);
        mFilter.accumulateAsIs(rs.mag);

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
