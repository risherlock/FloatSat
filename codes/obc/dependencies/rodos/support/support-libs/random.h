#include <stdint.h>

/**
*
* @date 2008/06/17 , 2020/02/02
* @author Sergio Montenegro
*
* @brief Random functions
*/


namespace RODOS {

extern uint64_t uint64Rand(); // in this implementation https://de.wikipedia.org/wiki/KISS_(Zufallszahlengenerator)
extern uint32_t uint32Rand(); // the lower bits from uint64Rand()

extern double drandPositive();            // 0 ..1
extern double drandPositive(double gain); // 0 .. gain
extern double drand();                    // -1 .. 1
extern double drand(double gain);         // -gain .. gain
extern void   setRandSeed(uint64_t x);    // sets only one of 4 possible seeds

} // namespace RODOS
