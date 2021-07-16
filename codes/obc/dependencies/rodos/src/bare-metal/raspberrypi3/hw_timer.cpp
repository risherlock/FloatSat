/*
 * @file hw_timer.cpp
 * @date 2016/08/25
 * @author Michael Ruffer, modified by Johannes Freitag and Michael Zehrer
 *
 * Copyright 2012-2016 University of Wuerzburg
 *
 * @brief Timer for system time and preemption
 */
#include <rodos.h>
#include <hw_specific.h>

#include "include/bcm2837.h"
#include "include/asm_defines.h"
#include "include/platform-parameter.h"

namespace RODOS {

/** 
 * the timer interval
 */
int64_t Timer::microsecondsInterval = PARAM_TIMER_INTERVAL;

void Timer::init() {
    uint32_t count;
    // get current counter value
    count = read32(SYSTEM_TIMER_CNT_LOW);
    // add period
    count += static_cast<uint32_t>(Timer::microsecondsInterval);

    // set compare value
    write32(SYSTEM_TIMER_COMPARE1, count);
    // reset counter interrupt
    write32(SYSTEM_TIMER_CONTROL, BIT(SYSTEM_TIMER_CONTROL_MATCH1));
    // activate timer interrupt
    write32(IRQ_ENABLE1, BIT(IRQ_SYSTEM_TIMER1));

    // enable interrupts global
    //hwEnableInterrupts(); // interrupts will be enabled with first context-load (IDLE-Thread)
}

/**
 * start timer
 * not necessary in this port because interrupts are disabled in swi routine contextSwitch
 */
void Timer::start() {
}

/**
 * stop timer
 * not necessary in this port because interrupt are disabled in swi routine contextSwitch
 */
void Timer::stop() {
}

/**
 * set timer interval
 */
void Timer::setInterval(const int64_t microsecondsInterval) {
    Timer::microsecondsInterval = microsecondsInterval;
}

/**
 * time at bootup
 */
int64_t initNanoTime = 0;

/**
 * RODOS time
 */
/*
uint64_t hwGetNanoseconds() {
    uint64_t low = read32(SYSTEM_TIMER_CNT_LOW);
    uint64_t high = ((uint64_t)read32(SYSTEM_TIMER_CNT_HIGH)) << 32;
    
    return (low | high) * 1000 - initNanoTime;
}
*/
int64_t hwGetNanoseconds() {
    uint64_t now;
    uint32_t systemTimerCntLowAdr = SYSTEM_TIMER_CNT_LOW;
    __asm volatile(
      "1: \n\t"
      /* read the higher 32 bit of the timer */
      "ldr r7, [%[stcntlowadr], #4]\n\t"
      /* read the lower 32 bit of the timer */
      "ldr r5, [%[stcntlowadr]]\n\t"
      /* read the higher 32 bit of the timer again */
      "ldr r6, [%[stcntlowadr], #4]\n\t"
      /* compare the two values of the high-register... */
      "cmp r6, r7\n\t"
      /* ... and repeat the whole step if they are not equal */
      "bne 1b\n\t"
      "str r5, [%[nowadr]]\n\t"
      "str r6, [%[nowadr], #4]\n\t"
      :
      : [nowadr] "r"(&now), [stcntlowadr] "r"(systemTimerCntLowAdr)
      : "r5", "r6", "r7", "memory");
    return static_cast<int64_t>(now * 1000) - initNanoTime;
}

/**
 * set time at bootup
 */
void hwInitTime() {
    initNanoTime = 0;
    initNanoTime = hwGetNanoseconds();
}

int64_t hwGetAbsoluteNanoseconds(void) {
    return hwGetNanoseconds(); // + timeAtStartup;
}

} /* namespace RODOS */
