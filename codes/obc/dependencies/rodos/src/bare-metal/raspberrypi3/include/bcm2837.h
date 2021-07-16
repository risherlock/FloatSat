/*
 * This file contains code derived from "arch/arm/mach-bcm2708/include/mach/platform.h"
 * of the Raspberry Pi Linux Kernel:
 *
 * Copyright (C) 2010 Broadcom
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * @file bcm2837.h
 * @date 2016/08/25
 * @author Michael Zehrer
 *
 * Copyright 2010 Broadcom
 * Copyright 2013-2016 University of Wuerzburg
 */
#include "stdint.h"

#ifndef BCM2837_H_
#    define BCM2837_H_

#    ifdef __cplusplus
extern "C" {
#    endif
__attribute__((always_inline)) inline void write32(uint32_t addr, uint32_t value) {
    (*((uint32_t*)addr)) = value;
}
__attribute__((always_inline)) inline uint32_t read32(uint32_t addr) {
    return *((uint32_t*)addr);
}

//write 32 bit to memory
extern void PUT32(uint32_t, uint32_t);
//get 32 bit from memory
extern uint32_t GET32(uint32_t);
//dummy for wait actions
extern void dummy(uint32_t);
//enable global interrupts
extern void enable_irq(void);
//disable global interrupts
extern void disable_irq(void);
//timer 1 interrupt handler
extern void TIM1_IRQHandler();
//read cpsr
extern uint32_t GETcpsr(void);
//read lr
extern uint32_t GETlr(void);
//activate mmu
extern void start_mmu(uint32_t, uint32_t);
//activate l1 cache
extern void start_l1cache();
#    ifdef __cplusplus
} // end extern "C"
#    endif

#    define BIT(n) (1 << (n))
/***********************************************************************
 * Definitions from document Broadcom BCM2835 ARM Peripherals
 **********************************************************************/

// On Raspberry Pi 1 this was 0x20000000
#    define PERIPHERALS_BASE 0x3F000000
#    define PERIPHERALS_SIZE 0x01000000

// *****************************************************************************
// GPIO definitions (from BCM2825 ARM Peripherals - Section 6)
// *****************************************************************************
#    define GPIO_BASE (PERIPHERALS_BASE + 0x00200000)

// GPIO Function selection
#    define GPFSEL0 (GPIO_BASE)        // GPIO Function Select 0
#    define GPFSEL1 (GPIO_BASE + 0x04) // GPIO Function Select 1
#    define GPFSEL2 (GPIO_BASE + 0x08) // GPIO Function Select 2
#    define GPFSEL3 (GPIO_BASE + 0x0C) // GPIO Function Select 3
#    define GPFSEL4 (GPIO_BASE + 0x10) // GPIO Function Select 4
#    define GPFSEL5 (GPIO_BASE + 0x14) // GPIO Function Select 5

// Pin Ouput Set Registers
#    define GPSET0 (GPIO_BASE + 0x1C) // GPIO Pin Output Set 0
#    define GPSET1 (GPIO_BASE + 0x20) // GPIO Pin Output Set 1
// Pin Output Clear Registers
#    define GPCLR0 (GPIO_BASE + 0x28) // GPIO Pin Output Clear 0
#    define GPCLR1 (GPIO_BASE + 0x2C) // GPIO Pin Output Clear 1
// Pin Level Registers
#    define GPLEV0 (GPIO_BASE + 0x34) // GPIO Pin Level 0
#    define GPLEV1 (GPIO_BASE + 0x38) // GPIO Pin Level 1
// Pin Event Detect Status Registers
#    define GPEDS0 (GPIO_BASE + 0x40) // GPIO Pin Event Detect Status 0
#    define GPEDS1 (GPIO_BASE + 0x44) // GPIO Pin Event Detect Status 1
// Pin Rising Edge Detect Enable Registers
#    define GPREN0 (GPIO_BASE + 0x4C) // GPIO Pin Rising Edge Detect Enable 0
#    define GPREN1 (GPIO_BASE + 0x50) // GPIO Pin Rising Edge Detect Enable 1
// Pin Falling Edge Detect Enable Registers
#    define GPFEN0 (GPIO_BASE + 0x58) // GPIO Pin Falling Edge Detect Enable 0
#    define GPFEN1 (GPIO_BASE + 0x5C) // GPIO Pin Falling Edge Detect Enable 1
// Pin High Detect Enable Registers
#    define GPHEN0 (GPIO_BASE + 0x64) // GPIO Pin High Detect Enable 0
#    define GPHEN1 (GPIO_BASE + 0x68) // GPIO Pin High Detect Enable 1
// Pin Low Detect Enable Registers
#    define GPLEN0 (GPIO_BASE + 0x70) // GPIO Pin Low Detect Enable 0
#    define GPLEN1 (GPIO_BASE + 0x74) // GPIO Pin Low Detect Enable 1
// Pin Async. Rising Edge Detect Registers
#    define GPAREN0 (GPIO_BASE + 0x7C) // GPIO Pin Async. Rising Edge Detect 0
#    define GPAREN1 (GPIO_BASE + 0x80) // GPIO Pin Async. Rising Edge Detect 1
// Pin Async. Falling Edge Detect Registers
#    define AFEN0 (GPIO_BASE + 0x88) // GPIO Pin Async. Falling Edge Detect 0
#    define AFEN1 (GPIO_BASE + 0x8C) // GPIO Pin Async. Falling Edge Detect 0
// Pin Pull-up/down Enable Register
#    define GPPUD (GPIO_BASE + 0x94) // GPIO Pin Pull-up/down Enable
// Pin Pull-up/down Enable Clock Registers (Must be used in conjunction with GPPUD)
#    define GPPUDCLK0 (GPIO_BASE + 0x98) // GPIO Pin Pull-up/down Enable Clock 0
#    define GPPUDCLK1 (GPIO_BASE + 0x9C) // GPIO Pin Pull-up/down Enable Clock 1

// Function Select Mask and Values
#    define GPIO_GPFSEL_MASK 7 // 3 bit for each PIN
#    define GPIO_GPFSEL_IN 0
#    define GPIO_GPFSEL_OUT 1
#    define GPIO_GPFSEL_ALT0 4
#    define GPIO_GPFSEL_ALT1 5
#    define GPIO_GPFSEL_ALT2 6
#    define GPIO_GPFSEL_ALT3 7
#    define GPIO_GPFSEL_ALT4 3
#    define GPIO_GPFSEL_ALT5 2

// Pull-up/down Enable Mask and Values
#    define GPIO_GPPUD_MASK 3 // 2 bit
#    define GPIO_GPPUD_NONE 0 // disable pull-up/down
#    define GPIO_GPPUD_DOWN 1 // Enable Pull Down control
#    define GPIO_GPPUD_UP 2   // Enable Pull Up control


// *****************************************************************************
// AUX (UART1 = miniUART, SPI1 and SPI2) (from BCM2825 ARM Peripherals - Section 2)
// *****************************************************************************
#    define AUX_BASE (PERIPHERALS_BASE + 0x00215000)
#    define AUX_IRQ (AUX_BASE)            // Auxiliary Interrupt status (3 bit)
#    define AUX_ENABLES (AUX_BASE + 0x04) // Auxiliary enables (3 bit)

#    define AUX_MU_IO_REG (AUX_BASE + 0x40)   // Mini Uart I/O Data (8 bit)
#    define AUX_MU_IER_REG (AUX_BASE + 0x44)  // Mini Uart Interrupt Enable (8 bit)
#    define AUX_MU_IIR_REG (AUX_BASE + 0x48)  // Mini Uart Interrupt Identify (8 bit)
#    define AUX_MU_LCR_REG (AUX_BASE + 0x4C)  // Mini Uart Line Control (8 bit)
#    define AUX_MU_MCR_REG (AUX_BASE + 0x50)  // Mini Uart Modem Control (8 bit)
#    define AUX_MU_LSR_REG (AUX_BASE + 0x54)  // Mini Uart Line Status (8 bit)
#    define AUX_MU_MSR_REG (AUX_BASE + 0x58)  // Mini Uart Modem Status (8 bit)
#    define AUX_MU_SCRATCH (AUX_BASE + 0x5C)  // Mini Uart Scratch (8 bit)
#    define AUX_MU_CNTL_REG (AUX_BASE + 0x60) // Mini Uart Extra Control (8 bit)
#    define AUX_MU_STAT_REG (AUX_BASE + 0x64) // Mini Uart Extra Status (8 bit)
#    define AUX_MU_BAUD_REG (AUX_BASE + 0x68) // Mini Uart Baudrate (16 bit)

#    define AUX_SPI0_CNTL0_REG (AUX_BASE + 0x80) // SPI 1 Control register 0 (32 bit)
#    define AUX_SPI0_CNTL1_REG (AUX_BASE + 0x84) // SPI 1 Control register 1 (8 bit)
#    define AUX_SPI0_STAT_REG (AUX_BASE + 0x88)  // SPI 1 Status (32 bit)
#    define AUX_SPI0_IO_REG (AUX_BASE + 0x90)    // SPI 1 Data (32 bit)
#    define AUX_SPI0_PEEK_REG (AUX_BASE + 0x94)  // SPI 1 Peek (16 bit)

#    define AUX_SPI1_CNTL0_REG (AUX_BASE + 0xC0) // SPI 2 Control register 0 (32 bit)
#    define AUX_SPI1_CNTL1_REG (AUX_BASE + 0xC4) // SPI 2 Control register 1 (8 bit)
#    define AUX_SPI1_STAT_REG (AUX_BASE + 0xC8)  // SPI 2 Status (32 bit)
#    define AUX_SPI1_IO_REG (AUX_BASE + 0xD0)    // SPI 2 Data (32 bit)
#    define AUX_SPI1_PEEK_REG (AUX_BASE + 0xD4)  // SPI 2 Peek (16 bit)

// *****************************************************************************
// Interrupt definitions (from BCM2825 ARM Peripherals - Table 7.5 - Page 112)
// *****************************************************************************
#    define IRQ_BASIC (PERIPHERALS_BASE + 0x0000B200)
#    define IRQ_PEND1 (PERIPHERALS_BASE + 0x0000B204)
#    define IRQ_PEND2 (PERIPHERALS_BASE + 0x0000B208)
#    define IRQ_FIQ_CONTROL (PERIPHERALS_BASE + 0x0000B20C)
#    define IRQ_ENABLE1 (PERIPHERALS_BASE + 0x0000B210)
#    define IRQ_ENABLE2 (PERIPHERALS_BASE + 0x0000B214)
#    define IRQ_ENABLE_BASIC (PERIPHERALS_BASE + 0x0000B218)
#    define IRQ_DISABLE1 (PERIPHERALS_BASE + 0x0000B21C)
#    define IRQ_DISABLE2 (PERIPHERALS_BASE + 0x0000B220)
#    define IRQ_DISABLE_BASIC (PERIPHERALS_BASE + 0x0000B224)

// IRQ lines of BCM2837 peripherals
// (IRQs 0-63 are those shared between the GPU and CPU, IRQs 64-95 are CPU-specific)
// IRQs 0 to 31 appear in the IRQ_PEND1 register
#    define IRQ_SYSTEM_TIMER0 0 // Already used by the VideoCore GPU (Do not use)
#    define IRQ_SYSTEM_TIMER1 1
#    define IRQ_SYSTEM_TIMER2 2 // Already used by the VideoCore GPU (Do not use)
#    define IRQ_SYSTEM_TIMER3 3

#    define IRQ_AUX 29 // AUX (UART1, SPI1 and SPI2)

#    define IRQ_GPIO0 49 // Bank0
#    define IRQ_GPIO1 50 // Bank1
#    define IRQ_GPIO2 51 // Bank2 (Non existent in BCM2837)
#    define IRQ_GPIO3 52 // Any Bank (all banks interrupt to allow GPIO FIQ)
#    define IRQ_GPIO_ALL IRQ_GPIO3


// *****************************************************************************
// ARM-Timer definitions (from BCM2825 ARM Peripherals - Table 14.2 - Page 196)
// *****************************************************************************
#    define ARM_TIMER_BASE (PERIPHERALS_BASE + 0x0000B000)
#    define ARM_TIMER_LOAD (ARM_TIMER_BASE + 0x400)
#    define ARM_TIMER_VALUE (ARM_TIMER_BASE + 0x404)
#    define ARM_TIMER_CONTROL (ARM_TIMER_BASE + 0x408)
#    define ARM_TIMER_IRQ_CLEAR (ARM_TIMER_BASE + 0x40C)
#    define ARM_TIMER_RAW_IRQ (ARM_TIMER_BASE + 0x410)
#    define ARM_TIMER_MASKED_IRQ (ARM_TIMER_BASE + 0x414)
#    define ARM_TIMER_RELOAD (ARM_TIMER_BASE + 0x418)
#    define ARM_TIMER_PREDIVIDER (ARM_TIMER_BASE + 0x41C)
#    define ARM_TIMER_FREE_RUNNING_COUNTER (ARM_TIMER_BASE + 0x420)

/** 0 : 16-bit counters
    1 : 23-bit counter */
#    define ARM_TIMER_CTRL_23BIT (1 << 1)

#    define ARM_TIMER_CTRL_PRESCALE_1 (0 << 2)
#    define ARM_TIMER_CTRL_PRESCALE_16 (1 << 2)
#    define ARM_TIMER_CTRL_PRESCALE_256 (2 << 2)

/** 0 : Timer interrupt disabled
    1 : Timer interrupt enabled */
#    define ARM_TIMER_CTRL_INT_ENABLE (1 << 5)
#    define ARM_TIMER_CTRL_INT_DISABLE (0 << 5)

/** 0 : Timer disabled
    1 : Timer enabled */
#    define ARM_TIMER_CTRL_ENABLE (1 << 7)
#    define ARM_TIMER_CTRL_DISABLE (0 << 7)


// *****************************************************************************
// System-Timer definitions (from BCM2825 ARM Peripherals - Table 12.1 - Page 172)
// *****************************************************************************
#    define SYSTEM_TIMER_BASE (PERIPHERALS_BASE + 0x00003000)
/* System Timer Control/Status */
#    define SYSTEM_TIMER_CONTROL (SYSTEM_TIMER_BASE + 0x0)
/* System Timer Counter Lower 32 bits */
#    define SYSTEM_TIMER_CNT_LOW (SYSTEM_TIMER_BASE + 0x4)
/* System Timer Counter Higher 32 bits */
#    define SYSTEM_TIMER_CNT_HIGH (SYSTEM_TIMER_BASE + 0x8)
/* System Timer Compare 0 */
#    define SYSTEM_TIMER_COMPARE0 (SYSTEM_TIMER_BASE + 0xc)
/* System Timer Compare 1 */
#    define SYSTEM_TIMER_COMPARE1 (SYSTEM_TIMER_BASE + 0x10)
/* System Timer Compare 2 */
#    define SYSTEM_TIMER_COMPARE2 (SYSTEM_TIMER_BASE + 0x14)
/* System Timer Compare 3 */
#    define SYSTEM_TIMER_COMPARE3 (SYSTEM_TIMER_BASE + 0x18)

#    define SYSTEM_TIMER_CONTROL_MATCH0 0x0 // Already used by the VideoCore GPU (Do not use)
#    define SYSTEM_TIMER_CONTROL_MATCH1 0x1
#    define SYSTEM_TIMER_CONTROL_MATCH2 0x2 // Already used by the VideoCore GPU (Do not use)
#    define SYSTEM_TIMER_CONTROL_MATCH3 0x3


// *****************************************************************************
// UART (page 12)
// *****************************************************************************
#    define AUX_MU_IIR_RX_IRQ_PENDING ((GET32(AUX_MU_IIR_REG) & 0x07) == 0x04) // ??? why 0x04 and not 0x02
#    define AUX_MU_IIR_TX_IRQ_PENDING ((GET32(AUX_MU_IIR_REG) & 0x07) == 0x02) // ??? why 0x02 and not 0x01
#    define AUX_MU_LSR_RX_RDY (GET32(AUX_MU_LSR_REG) & (1 << 0))


/*-------------------NOT TESTED ON THE RASPBERRY PI 3 FROM HERE ON-------------------*/

// *****************************************************************************
// Power Management, Reset controller and Watchdog
// (derived from "arch/arm/mach-bcm2708/include/mach/platform.h" of the Raspberry Pi Linux Kernel)
// *****************************************************************************
#    define POWER_MANAGEMENT_BASE (PERIPHERALS_BASE + 0x00100000)
#    define POWER_MANAGEMENT_RSTC (POWER_MANAGEMENT_BASE + 0x1c)
#    define POWER_MANAGEMENT_RSTS (POWER_MANAGEMENT_BASE + 0x20)
#    define POWER_MANAGEMENT_WDOG (POWER_MANAGEMENT_BASE + 0x24)

#    define POWER_MANAGEMENT_WDOG_RESET 0000000000
#    define POWER_MANAGEMENT_PASSWORD 0x5a000000
#    define POWER_MANAGEMENT_WDOG_TIME_SET 0x000fffff
#    define POWER_MANAGEMENT_RSTC_WRCFG_CLR 0xffffffcf
#    define POWER_MANAGEMENT_RSTC_WRCFG_SET 0x00000030
#    define POWER_MANAGEMENT_RSTC_WRCFG_FULL_RESET 0x00000020
#    define POWER_MANAGEMENT_RSTC_RESET 0x00000102

// *****************************************************************************
// Broadcom Serial Controllers (BSC/I2C)
// *****************************************************************************

/* Only BSC0 is accessible from the RPi pi header.*/
#    define BSC0_ADDR (PERIPHERALS_BASE + 0x00205000)
#    define BSC1_ADDR (PERIPHERALS_BASE + 0x00804000)
#    define BSC2_ADDR (PERIPHERALS_BASE + 0x00805000)

#    define BSC_CLOCK_FREQ 250000000


#    define BSC_REG_C 0x0
#    define BSC_REG_S 0x4
#    define BSC_REG_DLEN 0x8
#    define BSC_REG_A 0xc
#    define BSC_REG_FIFO 0x10
#    define BSC_REG_DIV 0x14
#    define BSC_REG_DEL 0x18
#    define BSC_REG_CLKT 0x1c

/* I2C control flags */
#    define BSC_I2CEN BIT(15)
#    define BSC_INTR BIT(10)
#    define BSC_INTT BIT(9)
#    define BSC_INTD BIT(8)
#    define BSC_ST BIT(7)
#    define BSC_CLEAR BIT(4)
#    define BSC_READ BIT(0)

/* I2C status flags */
#    define BSC_TA BIT(0)   /** @brief Transfer active.*/
#    define BSC_DONE BIT(1) /** @brief Transfer done.*/
#    define BSC_TXW BIT(2)  /** @brief FIFO needs writing.*/
#    define BSC_RXR BIT(3)  /** @brief FIFO needs reading.*/
#    define BSC_TXD BIT(4)  /** @brief FIFO can accept data.*/
#    define BSC_RXD BIT(5)  /** @brief FIFO contains data.*/
#    define BSC_TXE BIT(6)  /** @brief FIFO empty.*/
#    define BSC_RXF BIT(7)  /** @brief FIFO full.*/
#    define BSC_ERR BIT(8)  /** @brief ACK error.*/
#    define BSC_CLKT BIT(9) /** @brief Clock stretch timeout.*/

/* Rising/Falling Edge Delay Defaults.*/
#    define BSC_DEFAULT_FEDL 0x30
#    define BSC_DEFAULT_REDL 0x30

/* Clock Stretch Timeout Defaults.*/
#    define BSC_DEFAULT_CLKT 0x40

#    define CLEAR_STATUS BSC_CLKT | BSC_ERR | BSC_DONE

#    define START_READ BSC_I2CEN | BSC_ST | BSC_CLEAR | BSC_READ
#    define START_WRITE BSC_I2CEN | BSC_ST


// *****************************************************************************
// ARM local interrupt controller (from QA7_rev3.4 - Page 7-8)
// *****************************************************************************
#    define ARM_LOCAL_BASE 0x40000000
#    define ARM_LOCAL_CONTROL (ARM_LOCAL_BASE + 0x000)
//004 unused
#    define ARM_LOCAL_PRESCALER (ARM_LOCAL_BASE + 0x008)
#    define ARM_LOCAL_GPU_INT_ROUTING (ARM_LOCAL_BASE + 0x00C)
#    define ARM_LOCAL_PM_ROUTING_SET (ARM_LOCAL_BASE + 0x010)
#    define ARM_LOCAL_PM_ROUTING_CLR (ARM_LOCAL_BASE + 0x014)
//018 unused
#    define ARM_LOCAL_TIMER_LS (ARM_LOCAL_BASE + 0x01C)
#    define ARM_LOCAL_TIMER_MS (ARM_LOCAL_BASE + 0x020)
#    define ARM_LOCAL_INT_ROUTING (ARM_LOCAL_BASE + 0x024)
//028 unknown
#    define ARM_LOCAL_AXI_COUNT (ARM_LOCAL_BASE + 0x02C)
#    define ARM_LOCAL_AXI_IRQ (ARM_LOCAL_BASE + 0x030)
#    define ARM_LOCAL_TIMER_CONTROL (ARM_LOCAL_BASE + 0x034)
#    define ARM_LOCAL_TIMER_WRITE (ARM_LOCAL_BASE + 0x038)
//03C unused
#    define ARM_LOCAL_TIMER_INT_CONTROL_CORE0 (ARM_LOCAL_BASE + 0x040)
#    define ARM_LOCAL_TIMER_INT_CONTROL_CORE1 (ARM_LOCAL_BASE + 0x044)
#    define ARM_LOCAL_TIMER_INT_CONTROL_CORE2 (ARM_LOCAL_BASE + 0x048)
#    define ARM_LOCAL_TIMER_INT_CONTROL_CORE3 (ARM_LOCAL_BASE + 0x04C)

#    define ARM_LOCAL_MAILBOX_INT_CONTROL_CORE0 (ARM_LOCAL_BASE + 0x050)
#    define ARM_LOCAL_MAILBOX_INT_CONTROL_CORE1 (ARM_LOCAL_BASE + 0x054)
#    define ARM_LOCAL_MAILBOX_INT_CONTROL_CORE2 (ARM_LOCAL_BASE + 0x058)
#    define ARM_LOCAL_MAILBOX_INT_CONTROL_CORE3 (ARM_LOCAL_BASE + 0x05C)

#    define ARM_LOCAL_IRQ_PENDING_CORE0 (ARM_LOCAL_BASE + 0x060)
#    define ARM_LOCAL_IRQ_PENDING_CORE1 (ARM_LOCAL_BASE + 0x064)
#    define ARM_LOCAL_IRQ_PENDING_CORE2 (ARM_LOCAL_BASE + 0x068)
#    define ARM_LOCAL_IRQ_PENDING_CORE3 (ARM_LOCAL_BASE + 0x06C)

#    define ARM_LOCAL_FIQ_PENDING_CORE0 (ARM_LOCAL_BASE + 0x070)
#    define ARM_LOCAL_FIQ_PENDING_CORE1 (ARM_LOCAL_BASE + 0x074)
#    define ARM_LOCAL_FIQ_PENDING_CORE2 (ARM_LOCAL_BASE + 0x078)
#    define ARM_LOCAL_FIQ_PENDING_CORE3 (ARM_LOCAL_BASE + 0x07C)

#    define ARM_LOCAL_MAILBOX0_SET_CORE0 (ARM_LOCAL_BASE + 0x080)
#    define ARM_LOCAL_MAILBOX1_SET_CORE0 (ARM_LOCAL_BASE + 0x084)
#    define ARM_LOCAL_MAILBOX2_SET_CORE0 (ARM_LOCAL_BASE + 0x088)
#    define ARM_LOCAL_MAILBOX3_SET_CORE0 (ARM_LOCAL_BASE + 0x08C)

#    define ARM_LOCAL_MAILBOX0_SET_CORE1 (ARM_LOCAL_BASE + 0x090)
#    define ARM_LOCAL_MAILBOX1_SET_CORE1 (ARM_LOCAL_BASE + 0x094)
#    define ARM_LOCAL_MAILBOX2_SET_CORE1 (ARM_LOCAL_BASE + 0x098)
#    define ARM_LOCAL_MAILBOX3_SET_CORE1 (ARM_LOCAL_BASE + 0x09C)

#    define ARM_LOCAL_MAILBOX0_SET_CORE2 (ARM_LOCAL_BASE + 0x0A0)
#    define ARM_LOCAL_MAILBOX1_SET_CORE2 (ARM_LOCAL_BASE + 0x0A4)
#    define ARM_LOCAL_MAILBOX2_SET_CORE2 (ARM_LOCAL_BASE + 0x0A8)
#    define ARM_LOCAL_MAILBOX3_SET_CORE2 (ARM_LOCAL_BASE + 0x0AC)

#    define ARM_LOCAL_MAILBOX0_SET_CORE3 (ARM_LOCAL_BASE + 0x0B0)
#    define ARM_LOCAL_MAILBOX1_SET_CORE3 (ARM_LOCAL_BASE + 0x0B4)
#    define ARM_LOCAL_MAILBOX2_SET_CORE3 (ARM_LOCAL_BASE + 0x0B8)
#    define ARM_LOCAL_MAILBOX3_SET_CORE3 (ARM_LOCAL_BASE + 0x0BC)

#    define ARM_LOCAL_MAILBOX0_CLR_CORE0 (ARM_LOCAL_BASE + 0x0C0)
#    define ARM_LOCAL_MAILBOX1_CLR_CORE0 (ARM_LOCAL_BASE + 0x0C4)
#    define ARM_LOCAL_MAILBOX2_CLR_CORE0 (ARM_LOCAL_BASE + 0x0C8)
#    define ARM_LOCAL_MAILBOX3_CLR_CORE0 (ARM_LOCAL_BASE + 0x0CC)

#    define ARM_LOCAL_MAILBOX0_CLR_CORE1 (ARM_LOCAL_BASE + 0x0D0)
#    define ARM_LOCAL_MAILBOX1_CLR_CORE1 (ARM_LOCAL_BASE + 0x0D4)
#    define ARM_LOCAL_MAILBOX2_CLR_CORE1 (ARM_LOCAL_BASE + 0x0D8)
#    define ARM_LOCAL_MAILBOX3_CLR_CORE1 (ARM_LOCAL_BASE + 0x0DC)

#    define ARM_LOCAL_MAILBOX0_CLR_CORE2 (ARM_LOCAL_BASE + 0x0E0)
#    define ARM_LOCAL_MAILBOX1_CLR_CORE2 (ARM_LOCAL_BASE + 0x0E4)
#    define ARM_LOCAL_MAILBOX2_CLR_CORE2 (ARM_LOCAL_BASE + 0x0E8)
#    define ARM_LOCAL_MAILBOX3_CLR_CORE2 (ARM_LOCAL_BASE + 0x0EC)

#    define ARM_LOCAL_MAILBOX0_CLR_CORE3 (ARM_LOCAL_BASE + 0x0F0)
#    define ARM_LOCAL_MAILBOX1_CLR_CORE3 (ARM_LOCAL_BASE + 0x0F4)
#    define ARM_LOCAL_MAILBOX2_CLR_CORE3 (ARM_LOCAL_BASE + 0x0F8)
#    define ARM_LOCAL_MAILBOX3_CLR_CORE3 (ARM_LOCAL_BASE + 0x0FC)

#endif /* BCM2837_H_ */
