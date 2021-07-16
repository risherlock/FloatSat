/*
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Michael Ruffer
 */

#pragma once

#include "genericIO.h"
#include "hal_gpio.h"


namespace RODOS {

constexpr size_t UART_BUF_SIZE = 256;

enum UART_PARAMETER_TYPE {
	UART_PARAMETER_BAUDRATE,
	UART_PARAMETER_HW_FLOW_CONTROL,
	UART_PARAMETER_ENABLE_DMA
//	UART_PARAMETER_RX_PIN,
//	UART_PARAMETER_TX_PIN,
//	UART_PARAMETER_RTS_PIN,
//	UART_PARAMETER_CTS_PIN
//  UART_PARAMETER_MODE
};

enum UART_STATUS_TYPE {
	UART_STATUS_RX_BUF_LEVEL,
	UART_STATUS_RX_ERROR
};

enum UART_IDX {         // LINUX            POSIX               STM32F4         STM32F429      SF2
	UART_IDX0 =0,       // "/dev/ttyUSB0"    "/dev/ttyUSB0"      not available  not available  uart0 rs485
	UART_IDX1,          // "/dev/ttyUSB1"    "/dev/ttyUSB1"      USART1         USART1         uart1 stdio
	UART_IDX2,          // "/dev/rfcomm0"    "/dev/rfcomm0"      USART2         USART2
	UART_IDX3,          // "/dev/rfcomm1"    "/dev/rfcomm1"      USART3         USART3
	UART_IDX4,          // "/dev/ttyS0"      "/dev/ttyS0"        UART4          UART4
	UART_IDX5,          // "/dev/ttyS1"      "/dev/ttyAMA0"      UART5          UART5
	UART_IDX6,          //                   "/dev/ttyUSB2"      USART6         USART6
	UART_IDX7,          //                   "/dev/ttyUSB3"                     UART7
	UART_IDX8           //                                                      UART8
};

class HW_HAL_UART;

class HAL_UART : public GenericIOInterface {

private:
	HW_HAL_UART* context;

/* public functions */
public:
	/* constructor */
	HAL_UART(UART_IDX uartIdx);
	/* as the SF2 doesn't have variable UART pins, this constructor just calls the simple constructor HAL_UART(UART_IDX uartIdx); */
	HAL_UART(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, GPIO_PIN rtsPin = GPIO_INVALID, GPIO_PIN ctsPin = GPIO_INVALID );

	virtual ~HAL_UART() {};

	/* Initialization of UART interface: mode 8N1, no HW flow control*/
	virtual int32_t init(uint32_t baudrate = 115200);

	/* disable interface and set all its registers and pins to its reset state */
	virtual void reset();

 	/* Configuration of UART interface AFTER initialization */
	virtual int32_t config(UART_PARAMETER_TYPE type, int32_t paramVal);

	virtual int32_t status(UART_STATUS_TYPE type);

	virtual bool isWriteFinished();
	virtual bool isDataReady();

	/* write()
	 * non-blocking -> if internal buffer is full it takes less than "len"
	 * TODO: UART write - transmit buffer & transmit interrupts for non-blocking operation
         * WARNING if the internal buffer is full, it will write less than "len". Please check Always the return value!
	 * @return number of bytes written
	 */
	virtual size_t write(const void* sendBuf, size_t len);

	/* read()
	 * non-blocking -> returns when uart-receive-buffer is empty or "recBuf" is full
	 * @return number of bytes read and stored into recBuf
	 */
	virtual size_t read(void* recBuf, size_t maxLen);

	virtual int16_t getcharNoWait(); 		// returns character on success else -1
	virtual int16_t putcharNoWait(uint8_t c); 	// returns c on success else -1
};

}

