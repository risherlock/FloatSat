#include "stdio.h"
#include "unistd.h"
#include "string.h"

#include "mask.h"

#define PRINTF printf

int main() {
    INIT_MASK();
    char val[100];

    for(int time = 0; time < 5; time++) {
        sprintf(val, "%6d", time);
        printMaskField("NOW", val);
        for(int cnt = 100; cnt < 130; cnt++) {
            sprintf(val, "%3d", cnt);
            printMaskField("CNT", val);
            fflush(stdout);  // <<<<<<<<<<<<<<<<< Do not forget!!
            usleep(50000);
        }
    }
    return 0;
}
