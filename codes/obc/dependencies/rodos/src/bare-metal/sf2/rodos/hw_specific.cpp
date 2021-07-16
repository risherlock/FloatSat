/**
 * @file hw_specific.cc
 * @date 2008/04/23 7:33
 * @port 2017/07/14 19:14
 * @author Miroslaw Sulejczak, modified by Martin Boekhoff, Christian Feichtinger
 *
 *
 * @brief all hardware specific stuff I have no better place for ...
 *
 */
#include <sys/stat.h>
//#include <errno.h>
#include "../CMSIS/m2sxxx.h"
#include "../drivers/mss_sys_services.h"
#include "../drivers/mss_watchdog.h"
#include "../hal/CortexM3/cache.h"

#include "rodos.h"

#include "default-platform-parameter.h"
#include "hw_specific.h"
#include "hal/hal_uart.h"

volatile long* contextT;

namespace RODOS {

// - on interrupt the cortex m3 saves automatically the context of the interrupted function
// - it saves the registers xPSR,PC,LR,r12,r3,r2,r1,r0 on stack
// - that is why it's not necessary that the compiler adds code to save the context, so
//   we have to add the attribute "naked" to avoid this
extern "C" {
void PendSV_Handler(void) __attribute__((naked));
void SVC_Handler(void) __attribute__((naked));
}

/** function to do hardware initialization 
 * -> hwInit() is called in main() (src/independent/main.cpp)
 */

// Instantiate UART interface
HAL_UART uart_stdout(UART_DEBUG);

/**
 *  abort execution
 */
void hwResetAndReboot() {
    isShuttingDown = true;
    NVIC_SystemReset();
    //hwInitWatchdog(0);
    while(1) {}
}

/**
 *  global interrupt disable
 */
void hwDisableInterrupts() {
    __disable_irq();
}

/**
 *  global interrupt enable
 */
void hwEnableInterrupts() {
    __enable_irq();
}

void hwInit() {
    schedulerRunning = false;
    /* Turn off the watchdog */
    SYSREG->WDOG_CR = 0;
    /* Make PendSV, SysTick and Timer2 the same priroity */
    NVIC_SetPriority(PendSV_IRQn, 255);
    NVIC_SetPriority(SysTick_IRQn, 255);

    //	SystemInit(); // is called in startup code before constructor call and main
    SystemCoreClockUpdate(); // update a global variable holding the system clock
    MSS_SYS_init(NULL);

    // enable instruction cache for the first 256MiB of the address space
    MSS_CC_set_region(CC_256M_LOWER);
    MSS_CC_enable();

    //Determine the Node Number from the first bytes of the chips UID
    extern int32_t myNodeNr;
    uint8_t     serialNum[16] = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                              0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
    MSS_SYS_get_serial_number(serialNum);
    myNodeNr = static_cast<long>(serialNum[15] | ((uint32_t)serialNum[14] << 8) | ((uint32_t)serialNum[3] << 13) |
               ((uint32_t)serialNum[12] << 24));

    //UART for stdout
    uart_stdout.init(115200);

    PRINTF("CPU: %ld MHz\n", SystemCoreClock / 1000000);
}


/**
 * In case a network is avialable, the corresponding node nr.
 */

// TW: already defind in rodos/src/independent/globaldata.cpp
//int32_t myNodeNr = -1 ;
//long getNodeNumber() { return myNodeNr; }

extern "C" {
/*
 * SVC_Handler is only used to start the first thread
 * - We do this procedure to load rest of context of first thread (idleThread) and switch to PSP
 * -> PSP (Process Stack Pointer) is used in threads
 * -> MSP (Main Stack Pointer) is used in exception handlers (so in scheduler)
 * - MSP is the default stack pointer after reset
 */
void SVC_Handler(void) {
    __asm volatile(
      "	ldr	r1, =contextT				\n" // after start "contextT" holds the stack pointer of first thread
      "	ldr r0, [r1]					\n" // load stack pointer of first thread (idleThread)
      "	ldmia r0!, {r4-r11, r14}		\n" // Pop the registers that are not automatically saved on exception entry and the critical nesting count.
      "	msr psp, r0						\n" // load stack pointer of first thread to PSP -> PSP is used after leaving ISR
      "	bx r14							\n"
      "									\n"
      "	.align 2						\n");
}

/* When the ISR is entered the following registered are saved automatically:
 * FPSCR, S15 ... S0, xPSR, PC, LR, R12, R3, R2, R1, R0
 * -> the other registers (R11 ... R4, S31 ... S16) must be saved and restored manually
 * -> very helpful document: "PM0214 Programming manual - STM32F3xxx and STM32F4xxx Cortex-M4 programming manual" page 42
 */
void PendSV_Handler(void) {
    __asm volatile(
      "	cpsid f								\n" // disable all interrupts
      "	mrs r0, psp							\n" // load stack pointer of interrupted thread in r0 -> r0 is used to save all not automatically saved core and fpu registers
      "										\n"
#if(__FPU_PRESENT == 1) && (__FPU_USED == 1)
      "	tst r14, #0x10						\n" // Is the task using the FPU context?  If so, push high vfp registers.
      "	it eq								\n"
      "	vstmdbeq r0!, {s16-s31}				\n" // save FPU registers on stack of interrupted thread
#endif
      "										\n"
      "	stmdb r0!, {r4-r11, r14}			\n" // save core registers on stack of interrupted thread
      "										\n"
      "	bl schedulerWrapper					\n"
      "										\n"
      "	ldr r1, =contextT					\n" // "contextT" holds the stack pointer of next thread
      "	ldr r0, [r1]						\n" // load stack pointer of next thread
      "										\n"
      "	ldmia r0!, {r4-r11, r14}			\n" // Pop the core registers
      "										\n"
#if(__FPU_PRESENT == 1) && (__FPU_USED == 1)
      "	tst r14, #0x10						\n" // Is the task using the FPU context?  If so, pop the high vfp registers too
      "	it eq								\n"
      "	vldmiaeq r0!, {s16-s31}				\n"
#endif
      "										\n"
      "	msr psp, r0							\n" // load psp with stack pointer of next thread -> PSP (process stack pointer) is used after leaving ISR
      "	cpsie f								\n" // enable all interrupts
      "	bx r14								\n"
      "										\n"
      "	.align 2							\n");
}
}

/*
 * Initialize and start HW-watchdog
 * - it is used the IWDG (Independent Watchdog) of the STM32F4
 * - it is clocked by it's own independent low-speed clock (LSI)
 * - LSI can vary between 17 kHz and 47 kHz (32kHz typical)
 *   -> so it must be measured using Timer 5 -> done in hwInit
 * - max. interval@47kHz: 1/(47kHz/256) * 0xFFF = 22305 ms
 * - max. interval@32kHz: 1/(32kHz/256) * 0xFFF = 32768 ms
 * - max. interval@17kHz: 1/(17kHz/256) * 0xFFF = 61666 ms     TODO: implement Watchdog
 */
void hwInitWatchdog(int32_t intervalMilliseconds) {
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    //IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable );

    /* set IWDG counter clock prescaler depending on intervalMilliseconds to improve range and accuracy*/
    //uint32_t prescaler = 0;
    if(intervalMilliseconds < 1000) {
        //IWDG_SetPrescaler(IWDG_Prescaler_16 );
        //prescaler = 16;
    } else {
        //IWDG_SetPrescaler(IWDG_Prescaler_256 );
        //prescaler = 256;
    }

    /* Set counter reload value
	 Counter Reload Value = intervalMilliseconds / IWDG counter clock period
	 = intervalMilliseconds / (LsiFreq/prescaler)
	 */
    //double wdgClk = (double) LSIFrequency / (double) prescaler;
    //double intervalSec = (double) intervalMilliseconds / (double) 1000;
    //uint32_t reloadVal = intervalSec * wdgClk;
    //if (reloadVal > 0xFFF) {
    //	reloadVal = 0xFFF; // set to maximum reload value
    //}
    //IWDG_SetReload(reloadVal);

    /* Reload IWDG counter */
    //IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    //IWDG_Enable();
}

/**
 * trigger HW-watchdog -> must be done periodically with less than "intervalMilliseconds" ms
 */
void hwTriggerWatchdog() {
    //IWDG_ReloadCounter();
}

/**
 * @brief  This function handles TIM5 global interrupt during LSI measurement on startup.
 *         This is done only once in hwInit().
 * @param  None
 * @retval None
 */
//static volatile unsigned int PeriodValue = 0;
//static volatile bool captureRdy = false;
//static uint32_t CaptureNumber = 0;

extern "C" {
} // end extern "C"

} /* namespace RODOS */
