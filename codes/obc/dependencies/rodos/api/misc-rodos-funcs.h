
/**
* @file misc-rodos-funcs.h
* @date 2008/06/17 10:46
* @author Sergio MOntenegro
*
* @brief simple misc functions
*/

#pragma once


#include <stdint.h>
#include <stddef.h>


namespace RODOS {

extern bool isShuttingDown; // set by hwResetAndReboot(), read by destructors

/**
 * if a network and a gateway are present, my node number
 */
int32_t getNodeNumber();

int64_t getNumberOfReceivedMsgsFromNetwork();

/**
 * allocation of memory. No free of memory exists.
 * allocates static memory.
 * it shall not be unset after thread:run() begin
 */
extern void* xmalloc(size_t len);

extern void hwResetAndReboot();        ///<  End of Program -> reboot Hw dependent
extern void hwInitWatchdog(int32_t intervalMilliseconds);
extern void hwTriggerWatchdog();        ///<  for CPUS which provide a hardware watchdog
extern void hwDisableInterrupts();      // global interrupt disable - use carefully
extern void hwEnableInterrupts();       // global interrupt enable - use carefully
extern void deepSleepUntil(int64_t until); //< cpu core and periphery off until external interrupt or time "until"
extern bool isSchedulerRunning();       //< implemented in the platform dependent scheduler


/** Nop... no operation ... do nothing ***/
void nop( ... );

extern float getCpuLoad(); ///< value from 0 to 1, average from the last call


inline bool getbit(uint32_t bitmap, uint8_t bitIndex) { return (bitmap >> bitIndex) & 0x01; }

inline void setbit(uint32_t* bitmap, bool value, uint8_t bitIndex) {
    if(value) *bitmap |=  (0x01u  << bitIndex);
    else      *bitmap &= ~(0x01u  << bitIndex);
}

template <typename T>
constexpr T min(const T a, const T b) {
    return (a < b) ? a : b;
}

template <typename T>
constexpr T max(const T a, const T b) {
    return (a > b) ? a : b;
}

template <typename T>
constexpr T abs(const T a) {
    return (a >= 0) ? a : -a;
}

constexpr uint32_t uint32_tOnes(uint8_t n) {
    return (static_cast<uint32_t>(1)  << n) - static_cast<uint32_t>(1);
}

template <typename T>
constexpr bool inClosedInterval(const T& a, const T& min, const T& max) {
    return (min <= a) && (a <= max);
}

template <typename T>
constexpr bool inOpenInterval(const T& a, const T& min, const T& max) {
    return (min < a) && (a < max);
}

constexpr uint16_t chars2short(const char a, const char b) {
    return static_cast<uint16_t>((a << 8) + b);
}

}  // namespace


/**
 * MAIN() will be executed after initialisation and before
 * threads are started. RODOS provides an empty MAIN function.
 * The user may write its own MAIN function which will be
 * linked instead of the RODOS empty one.
 * outside of namespace RODOS!
 **/
void MAIN();

