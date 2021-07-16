/*
 * Copyright (c) 2011 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Michael Ruffer, Thomas Walter
 */
#include <new>
#include "rodos.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

#include "hal/hal_pwm.h"
//#include "yprintf.h"


#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

typedef enum
{
    TIM_CHAN1,
    TIM_CHAN2,
    TIM_CHAN3,
    TIM_CHAN4
} TIM_CHAN_TypeDef;


class HW_HAL_PWM{
public:
    HW_HAL_PWM(PWM_IDX idx);

	uint32_t increments;
	uint32_t Frequency;
	uint32_t pulseWidthInIncs;
	TIM_CHAN_TypeDef channel;
	TIM_TypeDef* timer;
	uint32_t timerClk;
    uint16_t GPIO_Pin;
    GPIO_TypeDef* GPIO_Port;
	int PwmIdx;



	void updateSettings();
};

HAL_PWM::HAL_PWM(PWM_IDX idx) {

	context = new (xmalloc(sizeof(HW_HAL_PWM))) HW_HAL_PWM(idx);
}

void HAL_PWM::setIdx(PWM_IDX idx) {

	context = new (xmalloc(sizeof(HW_HAL_PWM))) HW_HAL_PWM(idx);
}

int32_t HAL_PWM::init(uint32_t frequency, uint32_t increments) {

    if ((static_cast<int>(context->PwmIdx) < static_cast<int>(PWM_IDX00)) || (static_cast<int>(context->PwmIdx) > static_cast<int>(PWM_IDX15))) {
        return -1;
    }

	static char pwmInit[4] = { 0, 0, 0, 0 };
	char timerIdx = 1;

    /**** get the clock the timer is running at ****
     * -> it is needed later several times to calc appropriate timer settings
     */
	unsigned char timClockMultiplier = 0;
	RCC_ClocksTypeDef clocks;
	// if the APBx Prescaler is bigger than 1, the PCLKx is multiplied by 2 in front of the timer
    RCC_GetClocksFreq(&clocks);
    uint32_t pclkFrequency = 0;
    if (context->timer == TIM1){
        pclkFrequency = clocks.PCLK2_Frequency;
    }
    else {
        pclkFrequency = clocks.PCLK1_Frequency;
    }

    if (clocks.HCLK_Frequency / pclkFrequency > 1) {
        timClockMultiplier = 2;
    } else {
        timClockMultiplier = 1;
    }

    context->timerClk = pclkFrequency * timClockMultiplier;

    context->Frequency = frequency;

    uint32_t incMax = context->timerClk/context->Frequency;
    if (context->PwmIdx < PWM_IDX04 || context->PwmIdx > PWM_IDX07){ // Timer 1,3,4
        if (incMax > 0xFFFFul) incMax = 0xFFFFul;
    }
    if (increments > incMax) increments = incMax;

    context->increments = increments;
    //context->pulseWidthInIncs = increments/2; // default duty cycle = 50%
    context->pulseWidthInIncs = 1; // default duty cycle = minimum


	/**** configure GPIO for timer-Use ****/

	// Configure timer pin as output, alternate function, push-pull
	GPIO_InitTypeDef Gis;
	GPIO_StructInit(&Gis);
	Gis.GPIO_Mode = GPIO_Mode_OUT; // first, pin is configured as GPIO fixed to 0 and will later be activated with write()
	Gis.GPIO_OType = GPIO_OType_PP;
	Gis.GPIO_Speed = GPIO_Speed_100MHz;
	Gis.GPIO_PuPd = GPIO_PuPd_NOPULL;

	if (context->timer == TIM1) { // Enable GPIO clock and release reset
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
		RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOE , DISABLE);
		switch (context->channel) {
		case TIM_CHAN1:
			Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_9;
			context->GPIO_Port = GPIOE;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource9, GPIO_AF_TIM1);
			break; // PE9 -> CH1
		case TIM_CHAN2:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_11;
		    context->GPIO_Port = GPIOE;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource11, GPIO_AF_TIM1);
			break; // PE11 -> CH2
		case TIM_CHAN3:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_13;
		    context->GPIO_Port = GPIOE;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource13, GPIO_AF_TIM1);
			break; // PE13 -> CH3
		case TIM_CHAN4:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_14;
		    context->GPIO_Port = GPIOE;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource14, GPIO_AF_TIM1);
			break; // PE14 -> CH4
		}

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		timerIdx = 1;
	} else if (context->timer == TIM2) { // Enable GPIO clock and release reset
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphResetCmd( RCC_AHB1Periph_GPIOA, DISABLE);

		switch (context->channel) {
		case TIM_CHAN1:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_0;
		    context->GPIO_Port = GPIOA;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource0, GPIO_AF_TIM2);
			break; // PA0 -> CH1
		case TIM_CHAN2:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_1;
		    context->GPIO_Port = GPIOA;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource1, GPIO_AF_TIM2);
			break; // PA1 -> CH2
		case TIM_CHAN3:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_2;
		    context->GPIO_Port = GPIOA;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource2, GPIO_AF_TIM2);
			break; // PA2 -> CH3
		case TIM_CHAN4:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_3;
		    context->GPIO_Port = GPIOA;
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource3, GPIO_AF_TIM2);
			GPIO_Init(context->GPIO_Port, &Gis);
			break; // PA3 -> CH4
		}

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		timerIdx = 2;
	} else if (context->timer == TIM3) { // Enable GPIO clock and release reset
		RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB	| RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphResetCmd(	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB	| RCC_AHB1Periph_GPIOC, DISABLE);

		switch (context->channel) {
		case TIM_CHAN1:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_6;
		    context->GPIO_Port = GPIOA;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource6, GPIO_AF_TIM3);
			break; // PA6 -> CH1
		case TIM_CHAN2:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_7;
		    context->GPIO_Port = GPIOC;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource7, GPIO_AF_TIM3);
			break; // PC7 -> CH2
		case TIM_CHAN3:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_0;
		    context->GPIO_Port = GPIOB;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource0, GPIO_AF_TIM3);
			break; // PB0 -> CH3
		case TIM_CHAN4:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_1;
		    context->GPIO_Port = GPIOB;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource1, GPIO_AF_TIM3);
			break; // PB1 -> CH4
		}

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		timerIdx = 3;
	} else if (context->timer == TIM4) { // Enable GPIO clock and release reset
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_GPIOD, DISABLE);

		switch (context->channel) {
		case TIM_CHAN1:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_12;
		    context->GPIO_Port = GPIOD;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource12, GPIO_AF_TIM4);
			break; // PD12 -> CH1
		case TIM_CHAN2:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_13;
		    context->GPIO_Port = GPIOD;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource13, GPIO_AF_TIM4);
			break; // PD13 -> CH2
		case TIM_CHAN3:
		    Gis.GPIO_Pin = context->GPIO_Pin = GPIO_Pin_14;
		    context->GPIO_Port = GPIOD;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource14, GPIO_AF_TIM4);
			break; // PD14 -> CH3
		case TIM_CHAN4:
            Gis.GPIO_Pin = context->GPIO_Pin  = GPIO_Pin_15;
            context->GPIO_Port = GPIOD;
			GPIO_Init(context->GPIO_Port, &Gis);
			GPIO_PinAFConfig(context->GPIO_Port, GPIO_PinSource15, GPIO_AF_TIM4);
			break; // PD15 -> CH4
		}

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		timerIdx = 4;
	} else {
		return -1; // timer not supported
	}

	GPIO_ResetBits(context->GPIO_Port, context->GPIO_Pin);

	/* reset timer before new init, only if it's not already used as PWM source */
	if (pwmInit[timerIdx - 1] == 0) {
		TIM_DeInit(context->timer);
		pwmInit[timerIdx - 1] = 1;
	}

	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE); // is done in TIM_DeInit
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE); // is done in TIM_DeInit
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE); // is done in TIM_DeInit
	// RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE); // is done in TIM_DeInit

	//DBGMCU_Config(DBGMCU_TIM2_STOP, ENABLE);			// stop Timer during debug break

	if (context->timer == TIM1){
		TIM_CtrlPWMOutputs(context->timer, ENABLE); // this is only necessary for timer 1
	}

	TIM_InternalClockConfig(context->timer);        // timer runs with internal clock

    context->updateSettings();
	TIM_Cmd(context->timer, ENABLE);

	return 0;
}

int32_t HAL_PWM::config(PWM_PARAMETER_TYPE type, int32_t paramVal) {
    if ((static_cast<int>(context->PwmIdx) < static_cast<int>(PWM_IDX00)) || (static_cast<int>(context->PwmIdx) > static_cast<int>(PWM_IDX15))) {
        return -1;
    }

    switch (type) {
	case PWM_PARAMETER_INCREMENTS:{
	    uint32_t incMax = context->timerClk/context->Frequency;
	    if (context->PwmIdx < PWM_IDX04 || context->PwmIdx > PWM_IDX07){ // Timer 1,3,4
	        if (incMax > 0xFFFF) incMax = 0xFFFF;
	    }
		if (paramVal < 2 || static_cast<uint32_t>(paramVal) > incMax)
			return -1; // parameter invalid
		context->increments = static_cast<uint32_t>(paramVal);
		break;
	}
	case PWM_PARAMETER_FREQUENCY:
		if (paramVal < 0 || (static_cast<uint32_t>(paramVal)*context->increments) > context->timerClk)
			return -1; // parameter invalid
		context->Frequency = static_cast<uint32_t>(paramVal);
		break;
	}

	context->updateSettings();

	return 0;
}

void HAL_PWM::reset() {
	if ((static_cast<int>(context->PwmIdx) < static_cast<int>(PWM_IDX00)) || (static_cast<int>(context->PwmIdx) > static_cast<int>(PWM_IDX15))) {
        return;
    }

	// Reset Timer
	TIM_DeInit(context->timer);

	// Reset pwm pins: input
	GPIO_InitTypeDef Gis;
	GPIO_StructInit(&Gis);
	Gis.GPIO_Mode = GPIO_Mode_IN;

	if (context->timer == TIM1) {
		switch (context->channel) {
		case TIM_CHAN1:
			Gis.GPIO_Pin = GPIO_Pin_9;
			GPIO_Init(GPIOE, &Gis);
			break; // PE9 -> CH1
		case TIM_CHAN2:
			Gis.GPIO_Pin = GPIO_Pin_11;
			GPIO_Init(GPIOE, &Gis);
			break; // PE11 -> CH2
		case TIM_CHAN3:
			Gis.GPIO_Pin = GPIO_Pin_13;
			GPIO_Init(GPIOE, &Gis);
			break; // PE13 -> CH3
		case TIM_CHAN4:
			Gis.GPIO_Pin = GPIO_Pin_14;
			GPIO_Init(GPIOE, &Gis);
			break; // PE14 -> CH4
		}
	} else if (context->timer == TIM2) {
		switch (context->channel) {
		case TIM_CHAN1:
			Gis.GPIO_Pin = GPIO_Pin_0;
			GPIO_Init(GPIOA, &Gis);
			break; // PA0 -> CH1
		case TIM_CHAN2:
			Gis.GPIO_Pin = GPIO_Pin_1;
			GPIO_Init(GPIOA, &Gis);
			break; // PA1 -> CH2
		case TIM_CHAN3:
			Gis.GPIO_Pin = GPIO_Pin_2;
			GPIO_Init(GPIOA, &Gis);
			break; // PA2 -> CH3
		case TIM_CHAN4:
			Gis.GPIO_Pin = GPIO_Pin_3;
			GPIO_Init(GPIOA, &Gis);
			break; // PA3 -> CH4
		}
	} else if (context->timer == TIM3) {
		switch (context->channel) {
		case TIM_CHAN1:
			Gis.GPIO_Pin = GPIO_Pin_6;
			GPIO_Init(GPIOA, &Gis);
			break; // PA6 -> CH1
		case TIM_CHAN2:
			Gis.GPIO_Pin = GPIO_Pin_7;
			GPIO_Init(GPIOC, &Gis);
			break; // PC7 -> CH2
		case TIM_CHAN3:
			Gis.GPIO_Pin = GPIO_Pin_0;
			GPIO_Init(GPIOB, &Gis);
			break; // PB0 -> CH3
		case TIM_CHAN4:
			Gis.GPIO_Pin = GPIO_Pin_1;
			GPIO_Init(GPIOB, &Gis);
			break; // PB1 -> CH4
		}
	} else if (context->timer == TIM4) {
		switch (context->channel) {
		case TIM_CHAN1:
			Gis.GPIO_Pin = GPIO_Pin_6;
			GPIO_Init(GPIOB, &Gis);
			break; // PB6 -> CH1
		case TIM_CHAN2:
			Gis.GPIO_Pin = GPIO_Pin_7;
			GPIO_Init(GPIOB, &Gis);
			break; // PB7 -> CH2
		case TIM_CHAN3:
			Gis.GPIO_Pin = GPIO_Pin_8;
			GPIO_Init(GPIOB, &Gis);
			break; // PB8 -> CH3
		case TIM_CHAN4:
			Gis.GPIO_Pin = GPIO_Pin_9;
			GPIO_Init(GPIOB, &Gis);
			break; // PB9 -> CH4
		}
	}
	/*else
	 {
	 return -1; // timer not supported
	 }*/

}

int32_t HAL_PWM::write(uint32_t pulseWidthInIncs) {
	if ((static_cast<int>(context->PwmIdx) < static_cast<int>(PWM_IDX00)) || (static_cast<int>(context->PwmIdx) > static_cast<int>(PWM_IDX15))) {
        return -1;
    }

    GPIO_InitTypeDef Gis; // is used to configure output pin
    Gis.GPIO_Mode = GPIO_Mode_OUT; // GPIO mode to deactivate PWM output if necessary
    Gis.GPIO_OType = GPIO_OType_PP;
    Gis.GPIO_Speed = GPIO_Speed_100MHz;
    Gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    Gis.GPIO_Pin = context->GPIO_Pin;

	if (pulseWidthInIncs >= context->increments){
	    pulseWidthInIncs = context->increments - 1; // set to maximum, if necessary
	}

	if (pulseWidthInIncs <= 0){ // set output = 0
	    GPIO_Init(context->GPIO_Port, &Gis);
	    GPIO_ResetBits(context->GPIO_Port, context->GPIO_Pin);
	    return 0;
	}

	context->pulseWidthInIncs = pulseWidthInIncs;

	if ((context->timer == TIM1) || (context->timer == TIM2) || (context->timer == TIM3) || (context->timer == TIM4)) {
		switch (context->channel) {
		case TIM_CHAN1:
			TIM_SetCompare1(context->timer, context->increments - pulseWidthInIncs);
			break;
		case TIM_CHAN2:
			TIM_SetCompare2(context->timer, context->increments - pulseWidthInIncs);
			break;
		case TIM_CHAN3:
			TIM_SetCompare3(context->timer, context->increments - pulseWidthInIncs);
			break;
		case TIM_CHAN4:
			TIM_SetCompare4(context->timer, context->increments - pulseWidthInIncs);
			break;
		}
	} else {
		return -1;
	}

	if (pulseWidthInIncs > 0){ // activate output pin
        Gis.GPIO_Mode = GPIO_Mode_AF; // alternate function mode (PWM)
        GPIO_Init(context->GPIO_Port, &Gis);
	}

	upCallWriteFinished();

	return 0;
}



HW_HAL_PWM::HW_HAL_PWM(PWM_IDX idx){
    increments = 2;
    pulseWidthInIncs = 1;
    Frequency = 0;
    PwmIdx = -1;
    timer = NULL;
    channel = TIM_CHAN1;
    timerClk = 0;
    GPIO_Pin = 0;
    GPIO_Port = NULL;

    if ((static_cast<unsigned int>(idx) > static_cast<unsigned int>(PWM_IDX15))) {
        RODOS_ERROR("Invalid PWM Index\n");
        return;
    }

    PwmIdx = idx;

    // conversion from index to Timer and channel
    switch (PwmIdx / 4) {
    case 0:
        timer = TIM1;
        break;
    case 1:
        timer = TIM2;
        break;
    case 2:
        timer = TIM3;
        break;
    case 3:
        timer = TIM4;
        break;
    default:
        return;
    }

    channel = (TIM_CHAN_TypeDef) (PwmIdx % 4);
}



void HW_HAL_PWM::updateSettings(){
    // local variables to initialize timer
    TIM_TimeBaseInitTypeDef timStruct;
    TIM_OCInitTypeDef timOCInitStruct; // init structure for OC-modes

    if (Frequency == 0){
        // set to One Pulse Mode, so timer finishes last period completely before stop
        // -> this is necessary to have a defined level on output pin -> "low"
        // -> when we stop the timer hard, the level is undefined, depending on current counter value
        TIM_SelectOnePulseMode(timer,TIM_OPMode_Single);
        return;
    }else{
        TIM_SelectOnePulseMode(timer,TIM_OPMode_Repetitive);
    }

    // Prescaler
    // fCK_CNT = fCK_PSC/(TIM_Prescaler+1)
    // - it's important to set the prescaler properly, because timer is only a 16Bit counter with
    //   a maximum reload-value of 2^16 = 65536 (except timer2 -> 32Bit)
    int timPrescaler = static_cast<int>(timerClk / (Frequency *increments)) - 1;
    if (timPrescaler < 0) timPrescaler = 0;
    // e.g. 168000000/(1000*100)-1 = 1679

    timStruct.TIM_Prescaler = static_cast<uint16_t>(timPrescaler);

    timStruct.TIM_Period = increments;

    // timer mode is "count down"
    timStruct.TIM_CounterMode = TIM_CounterMode_Up;

    // TIM_ClockDivision = division ratio between the timer clock (CK_INT)
    // frequency and sampling clock used by the digital filters
    // -> not interesting for our purpose !!!
    timStruct.TIM_ClockDivision = TIM_CKD_DIV1;

    // only valid for TIM1 and TIM8 -> STM32F10x_HD and better
    timStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(timer, &timStruct);

    /* Preload enable */
    TIM_ARRPreloadConfig(timer, ENABLE);

    /**** pwm output settings ****/
    TIM_OCStructInit(&timOCInitStruct); // set init structure to default values

    timOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; /*!< Specifies the TIM mode.
     This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes */

    timOCInitStruct.TIM_OutputState = TIM_OutputState_Enable; /*!< Specifies the TIM Output Compare state.
     This parameter can be a value of @ref TIM_Output_Compare_state */

    timOCInitStruct.TIM_Pulse = increments - pulseWidthInIncs;

    timOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low; /*!< Specifies the output polarity.
     This parameter can be a value of @ref TIM_Output_Compare_Polarity */

    switch (channel) {
    case TIM_CHAN1:
        TIM_OC1Init(timer, &timOCInitStruct);
        TIM_OC1PreloadConfig(timer, TIM_OCPreload_Enable);
        break; // channel 1 init
    case TIM_CHAN2:
        TIM_OC2Init(timer, &timOCInitStruct);
        TIM_OC2PreloadConfig(timer, TIM_OCPreload_Enable);
        break; // channel 2 init
    case TIM_CHAN3:
        TIM_OC3Init(timer, &timOCInitStruct);
        TIM_OC3PreloadConfig(timer, TIM_OCPreload_Enable);
        break; // channel 3 init
    case TIM_CHAN4:
        TIM_OC4Init(timer, &timOCInitStruct);
        TIM_OC4PreloadConfig(timer, TIM_OCPreload_Enable);
        break; // channel 4 init
    default:
        return;
    }

    //TIM_Cmd(timer, ENABLE);
}

#ifndef NO_RODOS_NAMESPACE
}
#endif

