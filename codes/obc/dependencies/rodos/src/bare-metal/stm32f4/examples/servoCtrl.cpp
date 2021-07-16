/*
 * servoCtrl.cpp
 *
 * Created on: 30.10.2012
 * Author: Michael Ruffer
 *
 * To run this example you need to connect ValveDriverBoard and SensorBoard to STM32F4DISCOVERY.
 *
 */


#include "rodos.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "hal_pwm.h"


class ServoCtrlThread: public StaticThread<> {
	long long periode;
	HAL_PWM* pwm;
	int pwmStartValue;
public:
	ServoCtrlThread(const char* name, long long periode, HAL_PWM* pwm,
			int pwmStartValue) :
			StaticThread<>(name) {
		this->periode = periode;
		this->pwm = pwm;
		this->pwmStartValue = pwmStartValue;
	}

	void init() {
		pwm->init(50, 1000);
		pwm->write(pwmStartValue);
	}

	void run() {
		long long nextTime = NOW();
		int dutyCycle = pwmStartValue;
		int printBreakCnt = 0;

		while (1) {
			if (GPIOA->IDR & GPIO_Pin_0) {
				dutyCycle += 1;
				if (dutyCycle >= 125) // 12,5% of 20ms -> 2,5 ms
					dutyCycle = 30; // 3% of 20ms -> 0,6 ms
			}
			pwm->write(dutyCycle);

			printBreakCnt++;
			if (printBreakCnt >= 1*SECONDS/periode){
				xprintf("\nServo DutyCycle: %d\n\n", dutyCycle);
				printBreakCnt=0;
			}

			nextTime += periode;
			suspendCallerUntil(nextTime);
		}
	}
};

HAL_PWM pwm0(PWM_IDX00); // Timer 1 Chan 1
HAL_PWM pwm1(PWM_IDX01); // Timer 1 Chan 2
HAL_PWM pwm2(PWM_IDX02); // Timer 1 Chan 3
HAL_PWM pwm3(PWM_IDX03); // Timer 1 Chan 4

ServoCtrlThread servo1("servo1", 100 * MILLISECONDS, &pwm0, 30);
ServoCtrlThread servo2("servo2", 100 * MILLISECONDS, &pwm1, 30);
ServoCtrlThread servo3("servo3", 100 * MILLISECONDS, &pwm2, 30);
ServoCtrlThread servo4("servo4", 100 * MILLISECONDS, &pwm3, 30);


void printTimerRegister(TIM_TypeDef* timer) {
	xprintf("CR1: %0x\n", timer->CR1); /*!< TIM control register 1,              Address offset: 0x00 */
	xprintf("CR2: %0x\n", timer->CR2); /*!< TIM control register 2,              Address offset: 0x04 */
	xprintf("SMCR: %0x\n", timer->SMCR); /*!< TIM slave mode control register,     Address offset: 0x08 */
	xprintf("DIER: %0x\n", timer->DIER); /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
	xprintf("SR: %0x\n", timer->SR); /*!< TIM status register,                 Address offset: 0x10 */
	xprintf("EGR: %0x\n", timer->EGR); /*!< TIM event generation register,       Address offset: 0x14 */
	xprintf("CCMR1: %x\n", timer->CCMR1); /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
	xprintf("CCMR2: %x\n", timer->CCMR2); /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
	xprintf("CCER: %x\n", timer->CCER); /*!< TIM capture/compare enable register, Address offset: 0x20 */
	xprintf("CNT: %lx\n", timer->CNT); /*!< TIM counter register,                Address offset: 0x24 */
	xprintf("PSC: %x\n", timer->PSC); /*!< TIM prescaler,                       Address offset: 0x28 */
	xprintf("ARR: %lx\n", timer->ARR); /*!< TIM auto-reload register,            Address offset: 0x2C */
	xprintf("RCR: %x\n", timer->RCR); /*!< TIM repetition counter register,     Address offset: 0x30 */
	xprintf("CCR1: %lx\n", timer->CCR1); /*!< TIM capture/compare register 1,      Address offset: 0x34 */
	xprintf("CCR2: %lx\n", timer->CCR2); /*!< TIM capture/compare register 2,      Address offset: 0x38 */
	xprintf("CCR3: %lx\n", timer->CCR3); /*!< TIM capture/compare register 3,      Address offset: 0x3C */
	xprintf("CCR4: %lx\n", timer->CCR4); /*!< TIM capture/compare register 4,      Address offset: 0x40 */
	xprintf("BDTR: %x\n", timer->BDTR); /*!< TIM break and dead-time register,    Address offset: 0x44 */
	xprintf("DCR: %x\n", timer->DCR); /*!< TIM DMA control register,            Address offset: 0x48 */
	xprintf("DMAR: %x\n", timer->DMAR); /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
	xprintf("OR: %x\n", timer->OR); /*!< TIM option register,                 Address offset: 0x50 */
}

