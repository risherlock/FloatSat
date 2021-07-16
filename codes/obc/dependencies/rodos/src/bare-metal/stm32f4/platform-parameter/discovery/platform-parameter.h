
/**
 * @file params.h
 * @date 2012/08/20 16:13
 * @author Michael Ruffer
 *
 * Copyright 2012 University Wuerzburg
 *
 * @brief configuration parameters ...
 *
 */

#pragma once

// ___________________  only defines, not in namespace

/** Version Number */
#define OSVERSION "STM32F4 Cortex-M4 V1.0"

/*************** System Configuration *********/

/** system clocks
 * - APB1_CLK is fixed to CPU_CLK/4 (TIM2/3/4/5/6/7/12/13/14, USART2/3, UART4/5, SPI2/3, I2C1/2/3, CAN1/2, DAC1/2, WWDG)
 * - APB2_CLK is fixed to CPU_CLK/2 (TIM1/8/9/10/11, USART1/6, SPI1, ADC1/2/3, MMC)
 * - for more information see system_stm32f4xx.c
 */
// MINI-M4, STM32F4Discovery
#define CPU_CLK                         168   // 24,32,42,48,56,84,96,120,144,168 MHz

/** uart for printf **/
#define UART_DEBUG                      UART_IDX2, GPIO_053, GPIO_054 // PD5 and PD6
// #define UART_DEBUG                        UART_IDX1, GPIO_009, GPIO_010 // PA 9 PA 10 SKITH DEBUG am Programmer 
//#define UART_DEBUG                      UART_IDX3, GPIO_056, GPIO_057 // PD8 and PD9 USB at the extension board
//#define UART_DEBUG                      UART_IDX4, GPIO_000, GPIO_001 // PA0 and PA1
//#define UART_DEBUG                      UART_IDX5

#undef  XMALLOC_SIZE
#define XMALLOC_SIZE		40*1024
#undef  DEFAULT_STACKSIZE
#define DEFAULT_STACKSIZE	2000

/** stack size (in bytes) for scheduler
 * - ISRs and Scheduler are using the main stack (MSP)
 * - The array "char __schedulerStack__[SCHEDULER_STACKSIZE];" in scheduler.cpp is not used!
 * - So we must provide enough stack in the linker script (see "_Min_Stack_Size" in stm32_flash.ld)!
 */
#undef  SCHEDULER_STACKSIZE
#define SCHEDULER_STACKSIZE 		8


// Declare global variables with this to put them into the 64k-CoreCoupled Memory Block
// No DMA is possible. No initialization is possible,all gets set to zero.
#define PUT_INTO_CCRAM __attribute__ ((section (".bss_ccram")))

#undef  UDP_INCOMMIG_BUF_LEN
#define UDP_INCOMMIG_BUF_LEN          4 //number of "NetworkMessage (1300 Bytes each)" in FIFO


