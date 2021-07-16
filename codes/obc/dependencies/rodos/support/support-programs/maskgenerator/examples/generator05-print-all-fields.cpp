#include "stdio.h"
#include "stdlib.h"

#include "stdio.h"
#include "unistd.h"

int time;
int cnt;

#define NOW time
#define CNT cnt

#define PRINTMASK printf

#include "mask.h"


int main() {
    INIT_MASK();

    for(time = 0; time < 5; time++) {
        for(cnt = 100; cnt < 130; cnt++) {
            printMaskContent();
            fflush(stdout);  // <<<<<<<<<<<<<<<<< Do not forget!!
            usleep(50000);
        }
    }
    return 0;
}
