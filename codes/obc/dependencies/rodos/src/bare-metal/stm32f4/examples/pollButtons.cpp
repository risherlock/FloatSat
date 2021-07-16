/*
 * pollButtons.cpp
 *
 * Created on: 30.10.2012
 * Author: Michael Ruffer
 *
 * The switch used in this example is only available on SensorBoard - an extension board for STM32F4DISCOVERY.
 *
 */


#include "rodos.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>


class PollButtonsThread: public StaticThread<> {
	long long periode;
	unsigned int pin;
public:
	PollButtonsThread(const char* name, long long periode) :
			StaticThread<>(name) {
		this->periode = periode;
	}

	void init() {
		GPIO_InitTypeDef GPIO_InitStructure;

		/* GPIOD Periph clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		/* Configure PA0 (Button) input */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* Configure PD10 (Switch) input */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	}

	void run() {
		while (1) {
			//xprintf("Value of PA0 (Button): %ld\n", (GPIOA->IDR & GPIO_Pin_0));
			//xprintf("Value of PD10 (Switch): %ld\n", (GPIOD->IDR & GPIO_Pin_10) >> 10);

			suspendCallerUntil(NOW() + periode);
		}
	}
};

PollButtonsThread pb("pollButtonThread", 500 * MILLISECONDS);
/***********************************************************************/
