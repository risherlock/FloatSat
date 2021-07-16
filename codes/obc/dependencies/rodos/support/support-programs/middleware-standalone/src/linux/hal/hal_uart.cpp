/*
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg

 * All rights reserved.
 *
 * @author Michael Ruffer
 */
#include "hal/hal_uart.h"
#include "hal/hal_gpio.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>// for interrupt


#include "hw_udp.h"
#include "rodos-middleware.h"

#define MAX_READ_CHUNK_SIZE 16



#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

//================================================================================
//Mapping of UART IDs to linux device names
#define MAX_NUM_UARTS 6
const char *uartDeviceNames[6] = {"/dev/ttyUSB0","/dev/ttyUSB1","/dev/rfcomm0","/dev/rfcomm1","/dev/ttyS0","/dev/ttyS1"};
//================================================================================


void uart_sig_io_handler(int signo);


bool signal_init=false;
void sig_io_handler(int);

int uart_rxErrorStatus(int idx);

class HW_HAL_UART {
public:
	bool charReady;
	char lastChar;
	int fd;

	UART_IDX idx;
	HAL_UART* hal_uart;

};


HW_HAL_UART UART_contextArray[MAX_NUM_UARTS];

// constructor with pin assignment - dummy on Linux
HAL_UART::HAL_UART(UART_IDX uartIdx, GPIO_PIN txPin, GPIO_PIN rxPin, GPIO_PIN rtsPin, GPIO_PIN ctsPin) :  HAL_UART(uartIdx){

}

HAL_UART::HAL_UART(UART_IDX uartIdx){
	context = &UART_contextArray[uartIdx];

	context->idx = uartIdx;
	context->hal_uart = this;
	context->fd = -1;
}


/*
 * USART
 * - all USART will be initialized in 8N1 mode
 */
int HAL_UART::init(unsigned int iBaudrate) {

	struct termios t; ///< control structure for a general asynchronous interface

	context->charReady = false;
	context->lastChar  = 0;

	const char* devname = uartDeviceNames[context->idx];

	/* prepare termios */
	t.c_iflag = IGNBRK | IGNPAR;
	t.c_oflag = 0;
	t.c_cflag = CS8 | CREAD;// | CRTSCTS | CLOCAL;
	t.c_lflag = 0;  //ICANON;
	t.c_cc[VMIN] = 1; //TW:/* wait for 1 byte */
	t.c_cc[VTIME] = 0; //TW:/* turn off timer */

	switch(iBaudrate){
	case  38400:
		cfsetispeed(&t,B38400);
		break;
	case  57600:
		cfsetispeed(&t,B57600);
		break;
	case 115200:
		cfsetispeed(&t,B115200); /* normal shall be: B115200 Baud */
		break;
	default:
		xprintf("UART: baudrate not supported: %d\n",iBaudrate);
		return -1;
	}

	int fd = open(devname,O_RDWR  | O_NDELAY);
	context->fd=fd;
	if (fd==-1) {
		xprintf("UART: cannot open file: %s\n",devname);
		return -1;
	}

    //cfmakeraw(&t);
	tcsetattr(fd,TCSANOW, &t);

	fcntl(fd, F_SETOWN, getpid());
	fcntl(fd, F_SETFL, O_NONBLOCK|O_ASYNC);

	init_sigio_handler();

    //xprintf("UART: %s initialized with: %d\n",devname,iBaudrate);

	return 0;
}





int HAL_UART::config(UART_PARAMETER_TYPE type, int paramVal) {

	switch (type) {
		case UART_PARAMETER_BAUDRATE:
			if (paramVal > 0)
			{
				reset();
				init(paramVal);
			}
			else {return -1;}
			break;

		case UART_PARAMETER_HW_FLOW_CONTROL:
			return -1;

		case UART_PARAMETER_ENABLE_DMA:
			return -1;

		default:
			return -1;
	}

	return 0;
}


void HAL_UART::reset(){
    close(context->fd);
}


int HAL_UART::read(char* buf, int size) {
	int bytesRed=0;
	if(size <=0) return 0;



	if(size >0){
		int retval;
		retval=::read(context->fd,buf,size);
		if(retval < 0) return 0;
		bytesRed = retval;
	}

	return bytesRed;
}


int HAL_UART::write(const char* buf, int size) {
	if(size <=0) return 0;
	int retval;

	retval=::write(context->fd,buf,size);
	if(retval <0) return 0;

	return retval;
}


int HAL_UART::getcharNoWait() {
	char c = 0;

	if (read(&c,1) > 0)
	{
		return (int)c;
	}
	else
	{
		return -1;
	}
}


int HAL_UART::putcharNoWait(char c) {
	if(write(&c,1)>0){
		return c & 0xFF;
	}else{
		return -1;
	}
}


int HAL_UART::status(UART_STATUS_TYPE type) {

	UART_IDX idx = context->idx;

	if ((idx < 1) || (idx > 5)) {return -1;}

	switch (type)
	{
		case UART_STATUS_RX_BUF_LEVEL:
			return -1;

		case UART_STATUS_RX_ERROR:
			return -1;
		//...
		default: return -1;
	}
}


bool HAL_UART::isWriteFinished() {
	//return true;
	fd_set fdset;
		FD_ZERO(&fdset);
		FD_SET(context->fd,&fdset);

		timeval tval;
		tval.tv_sec=0;
		tval.tv_usec=0;

		// check if  fd can be written
		if(select(context->fd+1,0,&fdset,0,&tval) > 0)
			return true;
		else
			return false;

}


bool HAL_UART::isDataReady() {


	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(context->fd,&fdset);

	timeval tval;
	tval.tv_sec=0;
	tval.tv_usec=0;

	// check if there is something to read on fd
	if(select(context->fd+1,&fdset,0,0,&tval) > 0)
		return true;
	else
		return false;


}



void uart_sig_io_handler(int signo) {
	//PRINTF("UART IRQ\n");
	for(int i=0;i< MAX_NUM_UARTS;i++){
		HAL_UART* uart = UART_contextArray[i].hal_uart;
		if(uart){
			if(uart->isDataReady()){
				uart->upCallDataReady();
			}
			if(uart->isWriteFinished()){
				uart->upCallWriteFinished();
			}

		}

	}

}



#ifndef NO_RODOS_NAMESPACE
}
#endif

