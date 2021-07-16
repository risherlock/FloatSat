/*
 * motorCtrl.cpp
 *
 * Created on: 30.10.2012
 * Author: Michael Ruffer
 *
 * To run this example you need to connect ValveDriverBoard and SensorBoard to STM32F4DISCOVERY.
 *
 */



#include "rodos.h"
#include "stm32f4xx_conf.h"
#include "hal/hal_pwm.h"


HAL_PWM pwm10(PWM_IDX10); // Timer 3 Chan 3

class MotorCtrlThread: public StaticThread<> {
	long long periode;
public:
	MotorCtrlThread(const char* name, long long periode) :
			StaticThread<>(name) {
		this->periode = periode;
	}

	void init() {
		pwm10.init(); // defaults: f=1khz, steps=256

		GPIO_InitTypeDef GPIO_InitStructure;

		/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

		/* Configure PE10&PE12 (INA/INB of H-Bridge) */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		GPIOE->ODR |= GPIO_Pin_10 | GPIO_Pin_12; // set both pins high -> brake motor
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	}

	void run() {
		int dutyCycle = 0;
		while (1) {
			// increase duty cycle (rotation speed) by pressing "USERBT" button
			if (GPIOA->IDR & GPIO_Pin_0) {
				dutyCycle += 10;
				if (dutyCycle >= 256)
					dutyCycle = 0;
			}

			// change direction by switching "USERSW" switch
			if ((GPIOD->IDR & GPIO_Pin_10) >> 10) {
				GPIOE->ODR |= GPIO_Pin_10; // set PE10
				GPIOE->ODR &= ~(GPIO_Pin_12); // clear PE12
			} else {
				GPIOE->ODR |= GPIO_Pin_12; // set PE12
				GPIOE->ODR &= ~(GPIO_Pin_10); // clear PE10
			}

			pwm10.write(dutyCycle);
			//xprintf("\nDutyCycle: %d\n\n", dutyCycle);

			suspendCallerUntil(NOW() + periode);
		}
	}
};

MotorCtrlThread m1("MotorCtrl", 100 * MILLISECONDS);
/***********************************************************************/
