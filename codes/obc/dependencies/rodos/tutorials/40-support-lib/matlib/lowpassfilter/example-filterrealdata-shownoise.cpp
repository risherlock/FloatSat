
#include "rodos.h"
#include "signalprocessing.h"

#include "rawsensors.h"

int printfVerbosity = 0;  // Avoid printfs from RODOS Start


RawSensors rs;
LowpassFilterObject<Vector3D> aFilter, gFilter, mFilter;

void MAIN() { // RODOS Main
    printfVerbosity = 100;  
    aFilter.historyLen = 4; // 4 optimal for 200 hz sample, 50 hz vibration.
    gFilter.historyLen = 4; // experimental: 4 reduces noise factor 5. and 8 reduces factor 8
    mFilter.historyLen = 4; // experimental: 4 has delay of one cyle, 8 has delay of 3 clycles

    PRINTF("all = [;\n");
    while(rs.read()) {
        aFilter.accumulateAsIs(rs.acc);
        gFilter.accumulateAsIs(rs.gyro);
        mFilter.accumulateAsIs(rs.mag);

        PRINTF(" %f %f ",   aFilter.noiseIndex, aFilter.noiseAfterFilter);
        PRINTF(" %f %f ",   gFilter.noiseIndex, gFilter.noiseAfterFilter);
        PRINTF(" %f %f\n",  mFilter.noiseIndex, mFilter.noiseAfterFilter);
    }

    PRINTF("];\n\n");
    PRINTF("ap = all(:, 1);\n");
    PRINTF("aa = all(:, 2);\n");
    PRINTF("gp = all(:, 3);\n");
    PRINTF("ga = all(:, 4);\n");
    PRINTF("mp = all(:, 5);\n");
    PRINTF("ma = all(:, 6);\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(ap,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(aa,  '-b', 'LineWidth',1);\n");
    PRINTF("legend('preFileter', 'asterFilter');\n");
    PRINTF("title 'Noise accelerometer;'\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(gp,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(ga,  '-b', 'LineWidth',1);\n");
    PRINTF("legend('preFileter', 'asterFilter');\n");
    PRINTF("title 'Noise Gyro;'\n");

    PRINTF("figure\n");
    PRINTF("hold;\n");
    PRINTF("plot(mp,  '-r', 'LineWidth',1);\n");
    PRINTF("plot(ma,  '-b', 'LineWidth',1);\n");
    PRINTF("legend('preFileter', 'asterFilter');\n");
    PRINTF("title 'Noise Magnetometer;'\n");


    PRINTF("input cr_to_end;\n");

    printfVerbosity = 0;  

}
