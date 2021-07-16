/*
 * bluetoothTest.cpp
 *
 * Created on: 30.10.2012
 * Author: Michael Ruffer
 *
 * To run this example you need to connect the SensorBoard with Bluetooth-Modul to STM32F4DISCOVERY.
 *
 */


#include "rodos.h"
#include "hal/hal_uart.h"

HAL_UART uart2(UART_IDX2);

class BluetoothTest: public StaticThread<> {
	long long periode;
public:
	BluetoothTest(const char* name, long long periode) :
			StaticThread<>(name) {
		this->periode = periode;
	}

	void init(){
		uart2.init(115200);
		uart2.config(UART_PARAMETER_HW_FLOW_CONTROL,1);
	}

	void run() {
		long long nextTime = NOW();
		char ch[32];
		while (1) {
			// send test string via bluetooth
			uart2.write("Bluetooth\n",10);

			// print received character to debug output (uart3)
			int rxCnt = uart2.read(ch,31);
			ch[rxCnt] = 0;
			if (rxCnt > 0){	xprintf("char over BT %s\n",ch);}

			nextTime += periode;
			suspendCallerUntil(nextTime);
		}
	}
};

BluetoothTest bt1("BluetoothTest", 1000 * MILLISECONDS);
/***********************************************************************/
