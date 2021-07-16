#include <stdint.h>

#include "random.h"
#include "rodos.h"

#define RUNS 10000000

class RandomTests : public StaticThread<> {
    void run() {
        for(int i = 0; i < 10; i++) {
            setRandSeed(123345);
            double min  = 1.0;
            double max  = 0.0;
            double mean = 0;
            double r;
            for(int j = 0; j < RUNS; j++) {
                r = drandPositive(10.0);
                mean += r / RUNS;
                if((j % 1000000) == 0) yield();
                if(r > max) max = r;
                if(r < min) min = r;
            }
            PRINTF("min: %1.4f   max: %1.4f   mean  %1.4f\n", min, max, mean);
        }
        hwResetAndReboot();
    }
} randomTest;
