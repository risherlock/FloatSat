#include <stdio.h>

#include "udp_common.h"


UDPOutLx udpOut(7022, (char*)"localhost");

int main([[gnu::unused]] int argc, [[gnu::unused]] char *argv[]) {

    printf("Sending to localhost:7022\n");
    char inputline[1300];
    while(1) {
        printf("Data to send:");

        if (fgets(inputline, 1300, stdin) == nullptr) {
            return -1;
        }

        udpOut.send(inputline, 1300);
    }
    return 0;
}
