/*
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Michael Ruffer
 */


#ifndef _HAL_UART_H_
#define _HAL_UART_H_


#include "genericIO.h"
#include "hal_gpio.h"



#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


#define UART_BUF_SIZE 256

enum UART_PARAMETER_TYPE {
	UART_PARAMETER_BAUDRATE,
	UART_PARAMETER_HW_FLOW_CONTROL,
	UART_PARAMETER_ENABLE_DMA
//	UART_PARAMETER_RX_PIN,
//	UART_PARAMETER_TX_PIN,
//	UART_PARAMETER_RTS_PIN,
//	UART_PARAMETER_CTS_PIN
	//UART_PARAMETER_MODE
};


enum UART_STATUS_TYPE {
	UART_STATUS_RX_BUF_LEVEL,
	UART_STATUS_RX_ERROR
};

enum UART_IDX {         // LINUX            POSIX               STM32F4         STM32F429
	UART_IDX0 =0,       // "/dev/ttyUSB0"    "/dev/ttyUSB0"      not available  not available
	UART_IDX1,          // "/dev/ttyUSB1"    "/dev/ttyUSB1"      USART1         USART1
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
	HAL_UART(UART_IDX uartIdx);
	HAL_UART(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, GPIO_PIN rtsPin = GPIO_INVALID, GPIO_PIN ctsPin = GPIO_INVALID );

	/* Initialization of uart interface: mode 8N1, no HW flow control*/
	int init(unsigned int baudrate = 115200);

	/* disable interface and set all its registers and pins to its reset state */
	void reset();

 	/* Configuration of uart interface AFTER initialization */
	int config(UART_PARAMETER_TYPE type, int paramVal);

	int status(UART_STATUS_TYPE type);


	bool isWriteFinished();
	bool isDataReady();

	/* write()
	 * blocking -> returns only if "sendBuf" is sent
	 * TODO: transmit buffer & transmit interrupts for non-blocking operation
	 */
	int write(const char* sendBuf, int len);
	/* read()
	 * non-blocking -> returns when uart-receive-buffer is empty or "recBuf" is full
	 */
	int read(char* recBuf, int maxLen);

	int getcharNoWait(); 		// returns character on success else -1
	int putcharNoWait(char c); 	// returns c on success else -1


};




#ifndef NO_RODOS_NAMESPACE
}
#endif

#endif // _HAL_UART_H_
