/*
 * @file hw_timer.cpp
 * @date 2012/08/22
 * @author Michael Ruffer
 *
 * Copyright 2012 University of Wuerzburg
 *
 * @brief Timer for system time and preemption
 */
#include "rodos.h"
#include "hw_specific.h"

#include "default-platform-parameter.h"

#include "../drivers/mss_timer.h"

namespace RODOS {

static uint32_t timerClock = 1;

/***** Preemption Timer - Cortex "SysTick" counter *****
 *
 * Timer to force scheduler calls -> preemption
 * - cortex system counter is used -> "SysTick"
 * - 24Bit counter running at CPU_CLK/8 (125MHz/8 = 15,625MHz)
 * -> max. value: 0xFFF = 2^24 = 16.777.216
 * -> 1ms @ 15,625MHz: 15.625
 * -> 10ms  @ 15,625MHz: 156.250
 * -> 100ms  @ 15,625MHz: 1.562.500
 * -> !!! 1s  @ 15,625MHz: 15.625.000 ok, near an overflow
 */
extern long long timeToTryAgainToSchedule;
extern bool      isSchedulingEnabled;

extern "C" {

unsigned long long nanoTime = 0;

/*
 * Interrupt Service Routine for "SysTick" counter
 *
 * !!! Don't use "naked" for the ISR, because the system crashes
 * -> maybe because not all registers are saved automatically
 * -> it works when the compiler puts prologue and epilogue in the ISR
 *    -> but this can be a problem when ISRs can interrupt each other
 * -> this can happen when they don't have the same priority !!!
 */
void SysTick_Handler();
void SysTick_Handler() {
    nanoTime += PARAM_TIMER_INTERVAL * 1000; // 10M ns for each 10ms-tick

    if(!isSchedulingEnabled) {
        return;
    }

    long long timeNow = NOW();     // comment this out to improve performance, but: no time events any more
    TimeEvent::propagate(timeNow); // comment this out to improve performance, but: no time events any more

    if(NOW() < timeToTryAgainToSchedule) {
        return;
    }

    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // set SW-IRQ to call scheduler
}

/** \brief  System Tick Configuration

 This function initialises the system tick timer and its interrupt WITHOUT starting it
 Counter is in free running mode to generate periodical interrupts.

 \param [in]  ticks  Number of ticks between two interrupts
 \return          0  Function succeeded
 \return          1  Function failed
 */
static __INLINE uint32_t SysTick_Config_New(uint32_t ticks) {
    if(ticks > SysTick_LOAD_RELOAD_Msk) {
        return (1); /* Reload value impossible */
    }
    SysTick->LOAD = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;       /* set reload register */
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M3 System Interrupts */
    SysTick->VAL = 0;                                            /* Load the SysTick Counter Value */
    return (0);                                                  /* Function successful */
}

static __INLINE uint32_t SysTick_IRQEnable(void) {
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    return (0);
}

static __INLINE uint32_t SysTick_IRQDisable(void) {
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);

    return (0);
}

static __INLINE uint32_t SysTick_Enable(void) {
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | // if set clock source = CPU_CLK else CPU_CLK/8 (but this does not work on sf2)
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
    return (0);
}

static __INLINE uint32_t SysTick_Disable(void) {
    SysTick->CTRL = 0;

    return (0);
}

} // end extern "C"

/** 
 * the timer interval
 */
long long Timer::microsecondsInterval = PARAM_TIMER_INTERVAL;

void Timer::init() {
    Timer::microsecondsInterval = PARAM_TIMER_INTERVAL;
    timerClock                  = SystemCoreClock;
    SysTick_Config_New(
      static_cast<uint32_t>(((uint64_t)timerClock * (uint64_t)Timer::microsecondsInterval) / 1000000)); // initialization of systick timer, reload value: 1250000-1 -> generates an irq every 10ms with 125MHz sys clock
    SysTick_Enable();
}

/**
 * start timer
 */
void Timer::start() {
    SysTick_Enable();
}

/**
 * stop timer
 */
void Timer::stop() {
    SysTick_IRQDisable(); // only disable the interrupt for task swichting
}

/**
 * set timer interval
 */
void Timer::setInterval(const int64_t microsecondsInterval) {
    Timer::microsecondsInterval = microsecondsInterval;
}

/***** System Time ******
 *
 * timer is used to generate the system time
 * -> interrupt every 10ms
 * -> time resolution: 1/timerClock = 166,6ns
 * -> max. timerCount = timerClock/100 = 60 000 (IRQ every 10ms)
 */

//extern "C" {
/*
 * Interrupt Service Routine for Timer
 *
 * !!! Don't use "naked" for the ISR, because the system crashes
 * -> maybe because not all registers are saved automatically
 * -> it works when the compiler puts prologue and epilogue in the ISR
 *    -> but this can be a problem when ISRs can interrupt each other
 * -> this can happen when they don't have the same priority !!!
 */

int64_t hwGetNanoseconds(void) {

    unsigned long      count      = 0;
    unsigned long long returnTime = 0;

    // -> current time = nanoTime + 1 000 000 000 * countRegister/tim2Clock

    // Disable Interrupt is no solution here (wrong values caused by missed interrupt)

    // Read nanoTime twice, to make sure it has not changed while reading counter value
    do {
        returnTime = nanoTime;
        count      = timerClock - SysTick->VAL; // CMSIS/core_cmr.h
    } while(returnTime != nanoTime);

    /**
	 * Berechnung
	 * count ist der Timerwert zwischen den Aufrufen
	 * Alle 10 ms
	 *
	 */
    /** low precision
	 * - nanos = 166ns * count (for tim2Clock = 6MHz)
	 * - nanos = 11ns * count (for tim2Clock = 84MHz)
	 * ! startup time might be wrong due to integer overflow !
	 */
    //int nanos = 1000000000/timerClock * count;
    /** high precision
	 * - nanos = 125ns * count (for tim2Clock = 8MHz)
	 * - nanos = 8ns * count (for tim2Clock = 125MHz)
	 * - takes 4 times longer than low precision
	 */
    uint64_t nanos = ((uint64_t)count * 1000000) / (timerClock / 1000);

    return static_cast<int64_t>(returnTime + nanos);
}

void hwInitTime(void) {
    nanoTime = 0;
}

int64_t hwGetAbsoluteNanoseconds(void) {
    return hwGetNanoseconds(); // + timeAtStartup;
}

} /* namespace RODOS */
