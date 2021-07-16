/*
 * uartTest.cpp
 *
 * Created on: 30.10.2012
 * Author: Michael Ruffer
 *
 *
 */


#include "rodos.h"

namespace RODOS {
	extern HAL_UART uart_stdout;
}

HAL_GPIO ledGreen(GPIO_060); // green led
HAL_GPIO ledBlue(GPIO_063); // blue led

class UartIOEventReceiver : public IOEventReceiver{
public:
    void onWriteFinished() {
        static uint32_t ledLevel = 0;
        ledLevel ^= 1;
        ledGreen.setPins(ledLevel);
    }

    void onDataReady() {
        static uint32_t ledLevel = 0;
        ledLevel ^= 1;
        ledBlue.setPins(ledLevel);
    }
};

UartIOEventReceiver uartIOEventReceiver;

class UartReceiver: public StaticThread<>{
public:
    UartReceiver(const char* name) :	StaticThread<>(name) {
	}

	void init(){
        ledGreen.init(true,1,0);
        ledBlue.init(true,1,0);
	    uart_stdout.setIoEventReceiver(&uartIOEventReceiver);
	}

	void run() {
		char ch[32];

		while (1) {
			if (uart_stdout.read(ch,32) >0){
				xprintf("character received: %c\n",ch[0]);
			}

			//PRINTF("UartReceiver\n");

			uart_stdout.suspendUntilDataReady();
		}
	}
};

UartReceiver uartRX("UartTestRX");




class UartTransmitter: public StaticThread<> {
public:
    UartTransmitter(const char* name) :    StaticThread<>(name) {
    }

    void init(){
        uart_stdout.config(UART_PARAMETER_ENABLE_DMA,1);
    }

    void run() {
        int k = 0;
        AT(100*MILLISECONDS);

        while (1) {

            k++;
            if (k>20) k=1;


            for (int i=0;i<k;i++){
                uart_stdout.write("X",1);
            }

            uint64_t startTime = NOW();
            uart_stdout.suspendUntilWriteFinished();
            uint64_t suspendTime = NOW()-startTime;
            PRINTF("\nsuspend time: %d µs\n",(int)(suspendTime/1000));

            suspendCallerUntil(NOW()+1000*MILLISECONDS);
        }
    }
};

UartTransmitter uartTX("UartTestTX");

/***********************************************************************/
