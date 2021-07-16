/**
 * @file params.h
 * @date 2012/08/20 16:13
 * @author Michael Ruffer
 *         adapted in 2016 by Michael Zehrer
 *
 * Copyright 2012-2016 University Wuerzburg
 *
 * @brief configuration parameters ...
 */

#ifndef __PLATFORM_PARAMETER_H__
#define __PLATFORM_PARAMETER_H__

/** Version Number */
#define OSVERSION "Raspberry Pi 3"

/*************** System Configuration *********/

/** define UART for stdout with xprintf/printf */
/** PL011-UART = UART_IDX0; miniUART = UART_IDX1*/
#ifndef UART_DEBUG
#    define UART_DEBUG UART_IDX1
#endif

/** Memory for allocation (xmalloc) eg for all stacks ***/
#undef XMALLOC_SIZE
#define XMALLOC_SIZE 50 * 1024 * 1024

/** default stack size (in bytes) for threads */
#undef DEFAULT_STACKSIZE
#define DEFAULT_STACKSIZE 512 * 1024

/** stack size (in bytes) for scheduler thread
 * - The array "char __schedulerStack__[SCHEDULER_STACKSIZE];" in scheduler.cpp is not used!
 */
#undef SCHEDULER_STACKSIZE
#define SCHEDULER_STACKSIZE 8

#endif /* __PLATFORM_PARAMETER_H__ */
