/*
 * This file contains source code copyrighted by David Welch distributed under the terms of the MIT license.
 */

/**
 * @file hw_specific.cpp
 * @date 2013/06/01
 * @author Johannes Freitag, Michael Zehrer, Sergio Montenegro, David Welch
 *
 */
#include <inttypes.h>
#include <rodos.h>
#include <hal/hal_uart.h>
#include "hw_specific.h"

#include "include/bcm2837.h"
#include "include/asm_defines.h"
#include "include/mmu.h"
#include "include/platform-parameter.h"

volatile long* contextT;

namespace RODOS {

/*********************************************************************************************/
/* CONTEXT SWITCH AND INTERRUPT HANDLING */
/*********************************************************************************************/

extern int64_t timeToTryAgainToSchedule;
extern bool    isSchedulingEnabled;

extern "C" {

extern void UART1_IRQHandler();
extern void enable_dcache();
extern void enable_icache();
extern void enable_branch_predictor();

/*
 * Handles the hardware interrupt after saving the context and determines who has raised the interrupt
 */
void handleInterrupt(long* context) {
    contextT = context;
    // handles the timer interrupts
    if(read32(SYSTEM_TIMER_BASE) & BIT(SYSTEM_TIMER_CONTROL_MATCH1)) {
        // calc next ticktime (current time plus 'Timer::microsecondsInterval' <- is private)
        uint32_t nextTick = read32(SYSTEM_TIMER_CNT_LOW) + PARAM_TIMER_INTERVAL; //alle 10ms

        if(isSchedulingEnabled == true) {
            int64_t timeNow = NOW();

            TimeEvent::propagate(timeNow);
            if(NOW() > timeToTryAgainToSchedule) {
                // call scheduler with top of task stack
                schedulerWrapper(context);
            }
        }

        // set next tick time
        write32(SYSTEM_TIMER_COMPARE1, nextTick);

        // Write a one to the relevant bit to clear the match detect status bit
        // and the corresponding interrupt request line.
        // (BCM2835 ARM Peripherals - Page 172)
        write32(SYSTEM_TIMER_BASE, BIT(SYSTEM_TIMER_CONTROL_MATCH1));
    }

    //handles the uart interrupt
    if(AUX_MU_IIR_RX_IRQ_PENDING)
        UART1_IRQHandler();
}

} //end extern "C"

void enterSleepMode() {
}

/*********************************************************************************************/
/* HARDWARE INITIALIZATION */
/*********************************************************************************************/
HAL_UART uart_stdout(UART_DEBUG);

/** function to do hardware initialization
 * -> hwInit() is called in main() (src/independent/main.cpp)
 */
void hwInit() {
    schedulerRunning = false;

    // Init UART
    uart_stdout.init();

    init_mmu();

    enable_dcache();
    enable_icache();
    enable_branch_predictor();
}


/**
 *  abort execution
 */
void hwResetAndReboot() {
    isShuttingDown = true;
    xprintf("hw_resetAndReboot() not implemented yet\n");
}

/**
  *  global interrupt disable
  */
void hwDisableInterrupts() {
    // Disable FIQ and IRQ
    __asm volatile("cpsid if");
}

/**
  *  global interrupt enable
  */
void hwEnableInterrupts() {
    // Enable FIQ and IRQ
    __asm volatile("cpsie if");
}


/**
 * In case a network is avialable, the corresponding node nr.
 */
/*
int32_t myNodeNr = -1;

long getNodeNumber() {
	return myNodeNr;
}
*/

} /* namespace RODOS */
