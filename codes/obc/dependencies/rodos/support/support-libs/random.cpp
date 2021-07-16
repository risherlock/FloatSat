#include <stdint.h>

namespace RODOS {
/*
 * Author: George Marsaglia
 * See https://de.wikipedia.org/wiki/KISS_(Zufallszahlengenerator)
 */

struct RandomKiss {

    // State vector (ony 4 values) each can be used as seed (if != 0)
    static uint64_t x, y, z, c;

    static uint64_t KISS64() {
        uint64_t t;

        // Linearer Kongruenzgenerator
        x = 6906969069ULL * x + 1234567;

        // Xorshift
        y ^= y << 13;
        y ^= y >> 17;
        y ^= y << 43;

        // Multiply-with-carry
        t = (z << 58) + c;
        c = z >> 6;
        z += t;
        c += z < t;

        return x + y + z;
    }
};

// Initial seeds. you may change any, but do not set to 0
uint64_t RandomKiss::x = 1066149217761810ULL;
uint64_t RandomKiss::y = 362436362436362436ULL;
uint64_t RandomKiss::z = 1234567890987654321ULL;
uint64_t RandomKiss::c = 123456123456123456ULL;


//_________________________________________________ The interface in API :
// Double values normalized 0 ..1 or -1 .. 1
uint64_t uint64Rand()               { return RandomKiss::KISS64(); }
uint32_t uint32Rand()               { return (uint32_t)RandomKiss::KISS64(); }
double   drandPositive()            { return (double)RandomKiss::KISS64() / (double)UINT64_MAX; }
double   drandPositive(double gain) { return drandPositive() * gain; }
double   drand()                    { return drandPositive(2.0) - 1.0; }
double   drand(double gain)         { return drandPositive(2.0 * gain) - gain; }
void     setRandSeed(uint64_t x)    { RandomKiss::x = x; }

} // namespace RODOS
