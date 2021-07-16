
#include "stdio.h"

#include "stdint.h"
#include "my-app-telemetry.hh"

LanderTelemetry lanTM;
SkyObserverTelemety   skyTM;

char downlinkBuffer[600];

void sendTelemetry(char* buff, int bytesToSend) {
    buff[bytesToSend] = 0;
    printf("buf to send = '%s'\n", buff);
}

main() {
    
    int len;

    lanTM.timeNow      = 12345678LL;
    lanTM.descentSpeed = 0.001;
    lanTM.setLanded();


    len = lanTM.serialize(downlinkBuffer);
    sendTelemetry(downlinkBuffer, len);

    /************************************/

    skyTM.numberOfUFOS = 3;
    skyTM.speedOfUFO   = 300000000.0;

    len = skyTM.serialize(downlinkBuffer);
    sendTelemetry(downlinkBuffer, len);
};





