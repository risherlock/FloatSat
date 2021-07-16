#include "rodos.h"
static Application module01("LED_Test", 2001);

HAL_GPIO ledBlue(GPIO_060);
HAL_GPIO ledRed(GPIO_061);
HAL_GPIO ledOrange(GPIO_062);
HAL_GPIO ledGreen(GPIO_063);

class LEDTest: public StaticThread<> {
public:
	void init() {

		ledBlue.init(true);
		ledRed.init(true);
		ledOrange.init(true);
		ledGreen.init(true);
	}
	void run(){
		int8_t cnt=1;
		while(1){

		ledBlue.setPins(cnt&0x01);
		ledRed.setPins((cnt&0x02)>>1);
		ledOrange.setPins((cnt&0x04)>>2);
		ledGreen.setPins((cnt&0x08)>>3);
		cnt=cnt<<1;
		if(cnt>8)cnt=1;
	        suspendCallerUntil(NOW()+1000 * MILLISECONDS);
	        PRINTF("Cnt: %d\n",cnt);

		}//while
		}

};
LEDTest ledtest;




