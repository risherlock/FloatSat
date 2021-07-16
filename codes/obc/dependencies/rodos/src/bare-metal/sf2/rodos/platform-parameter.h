/**
 * @file params.h
 * @date 2017/02/28
 * @author Martin Boekhoff
 *
 * @brief configuration parameters ...
 *
 */
#ifndef __PLATFORM_PARAMETER_H__
#define __PLATFORM_PARAMETER_H__

//#include "userconfig.h"

// Work around issue with CMSIS files which lack the necessary
// includes for uint32_t and friends.
#include <stdint.h>

#include "../drivers_config/sys_config/sys_config.h"
#include "../CMSIS/system_m2sxxx.h"

/** Version Number */
#define OSVERSION "SmartFusion2 Cortex-M3 V0.2"

/*************** System Configuration *********/

/** the following define sets the UART used for debug outputs with xprintf
 *
 */
#ifndef UART_DEBUG
#    define UART_DEBUG UART_IDX1
#endif

/** Memory for allocation (xmalloc) e.g. for all thread stacks ***/
/** 20*1024 -> 20 Kilobyte **/
#undef XMALLOC_SIZE
#define XMALLOC_SIZE (30 * DEFAULT_STACKSIZE) + 32000

/** stack size (in bytes) for scheduler
 * - ISRs and Scheduler are using the main stack (MSP)
 * - The array "char __schedulerStack__[SCHEDULER_STACKSIZE];" in scheduler.cpp is not used!
 * - So we must provide enough stack in the linker script (see "_Min_Stack_Size" in stm32_flash.ld)!
 */
#undef SCHEDULER_STACKSIZE
#define SCHEDULER_STACKSIZE 8

/** time interval between timer interrupts in microseconds - max. 798000us (if CPU_CLK==168MHz)!!! */
#undef PARAM_TIMER_INTERVAL
#define PARAM_TIMER_INTERVAL 10000


#define _Min_Stack_Size MAIN_STACK_SIZE
#define _Min_Heap_Size MIN_HEAP_SIZE

// Declare global variables with this to put them into the 1024M-CoreCoupled Memory Block DDR3-RAM
// No DMA is possible. No initialisation is possible.
#define PUT_INTO_CCRAM __attribute__((section(".bss_ccram")))

#endif /* __PLATFORM_PARAMETER_H__ */
