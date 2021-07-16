#include "stdio.h"
#include "stdlib.h"

#include "stdio.h"
#include "unistd.h"

#include "mask.h"

#define PRINTF printf

int main() {
    INIT_MASK();

    for(int time = 0; time < 5; time++) {
        printf(NOW "%6d", time); // ****!!! be aware of this c++ speciality: "aaa" "bbb"
        for(int cnt = 100; cnt < 130; cnt++) {
            printf(CNT "%3d",  cnt);
            fflush(stdout);  // <<<<<<<<<<<<<<<<< Do not forget!!
            usleep(50000);
        }
    }
    return 0;
}
