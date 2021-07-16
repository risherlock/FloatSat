#include <new>
#include <string.h>

//#include "rodos.h"
#include "hal/hal_adc.h"
#include "rodos.h"

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx.h"

/*
 * HAL_ADC STM32F4
 * - there are 3 ADCs
 * - each ADC has 19 channels:
 * -> 3 channels for internal use (temperature, battery voltage)
 * -> 16 general purpose channels, BUT the channels share same pins!!!
 * -> so there are a maximum of 24 analog inputs (instead of 3x16)
 * pin      ADC         channel
 * ----------------------------
 * PA0      1,2,3       0
 * PA1      1,2,3       1
 * PA2      1,2,3       2
 * PA3      1,2,3       3
 * PA4      1,2         4
 * PA5      1,2         5
 * PA6      1,2         6
 * PA7      1,2         7
 * PB0      1,2         8
 * PB1      1,2         9
 * PC0      1,2,3       10
 * PC1      1,2,3       11
 * PC2      1,2,3       12
 * PC3      1,2,3       13
 * PC4      1,2         14
 * PC5      1,2         15
 *
 * PF6      3           4
 * PF7      3           5
 * PF8      3           6
 * PF9      3           7
 * PF10     3           8
 * PF3      3           9
 *
 * PF4      3           14
 * PF5      3           15
 */

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

#define STM32F4_MIN_ADC_IDX     ADC_IDX1
#define STM32F4_MAX_ADC_IDX     ADC_IDX3

#define STM32F4_MIN_ADC_CHAN    ADC_CH_000
#define STM32F4_MAX_ADC_CHAN    ADC_CH_018



class ADCChannelProps{
public:
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t RCC_AHB1Periph_GPIOx;
};


class HW_HAL_ADC{
    friend class HAL_ADC;
public:
    ADC_TypeDef *adc;
    int32_t idx;
    uint32_t RCC_APB2Periph_ADCx;
    int8_t channelsInUse[STM32F4_MAX_ADC_CHAN+1];

    bool initialized;


    HW_HAL_ADC(){/* do not initialize any member variables here !!! */}

    HW_HAL_ADC(ADC_IDX idx){
        if ( (idx < STM32F4_MIN_ADC_IDX) || (idx > STM32F4_MAX_ADC_IDX) ) return;

        initialized = false;

        this->idx=idx;

        switch(this->idx){
        case ADC_IDX1:
            adc = ADC1;
            RCC_APB2Periph_ADCx = RCC_APB2Periph_ADC1;
            break;
        case ADC_IDX2:
            adc = ADC2;
            RCC_APB2Periph_ADCx = RCC_APB2Periph_ADC2;
            break;
        case ADC_IDX3:
            adc = ADC3;
            RCC_APB2Periph_ADCx = RCC_APB2Periph_ADC3;
            break;
        default:
            break;
        }

        memset(channelsInUse,'\xFF',sizeof(channelsInUse));
    }

private:
    ADCChannelProps getChannelProperties(ADC_CHANNEL channel);
};


HW_HAL_ADC adcContextArray[STM32F4_MAX_ADC_IDX];


HAL_ADC::HAL_ADC(ADC_IDX idx) {
    if ( (static_cast<int>(idx) < static_cast<int>(STM32F4_MIN_ADC_IDX)) || (static_cast<int>(idx) > static_cast<int>(STM32F4_MAX_ADC_IDX)) ) {
        context->idx = STM32F4_MAX_ADC_IDX + 1; // set idx bigger than STM32F4_MAX_ADC_IDX to easily find out that idx is wrong (in all other methods)
        return;
    }

    context = new (&adcContextArray[idx-1]) HW_HAL_ADC(idx); // placement new to avoid dynamic memory allocation
}


int32_t HAL_ADC::init(ADC_CHANNEL channel){
    if (static_cast<int>(context->idx) > static_cast<int>(STM32F4_MAX_ADC_IDX)) return ADC_ERR_INVALID_INDEX;
    if ( (static_cast<int>(channel) > static_cast<int>(STM32F4_MAX_ADC_CHAN))) return ADC_ERR_INVALID_CHANNEL;

    context->channelsInUse[channel] = channel;

    if (channel < ADC_CH_016){
        ADCChannelProps props = context->getChannelProperties(channel);

        RCC_AHB1PeriphClockCmd(props.RCC_AHB1Periph_GPIOx, ENABLE);

        /*--- PORT X -------------------------------*/
        /* Configure PXX (ADC Channel XX) as analog input */
        GPIO_InitTypeDef gpioInit;
        gpioInit.GPIO_Pin = props.pin;
        gpioInit.GPIO_Mode = GPIO_Mode_AN;
        gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
        gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(props.port, &gpioInit);
    }


	if (context->initialized) return 0;

	RCC_APB2PeriphClockCmd(context->RCC_APB2Periph_ADCx, ENABLE);

	/* ADCX Configuration */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);
//    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
//    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
//    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//    ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(context->adc, &ADC_InitStructure);

	/* ADC Common configuration *************************************************/
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonStructInit(&ADC_CommonInitStructure);
//	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
//	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
//	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	//ADC_EOCOnEachRegularChannelCmd(context->adc,ENABLE); // "End Of Conversion" flag is set after each channel conversion (if converting a sequence)

	//ADC_Cmd(context->adc, ENABLE);	// Enable ADC

	context->initialized = true;

	return 0;
}


int32_t HAL_ADC::config(ADC_PARAMETER_TYPE type, int32_t value){
    if (static_cast<int>(context->idx) > static_cast<int>(STM32F4_MAX_ADC_IDX)) return ADC_ERR_INVALID_INDEX;

    ADC_InitTypeDef ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    switch(type){
    case ADC_PARAMETER_RESOLUTION:
        uint32_t resolution;
        switch(value){
        case 6: resolution = ADC_Resolution_6b; break;
        case 8: resolution = ADC_Resolution_8b; break;
        case 10: resolution = ADC_Resolution_10b; break;
        case 12: resolution = ADC_Resolution_12b; break;
        default: return ADC_ERR_PARAMETER_VALUE_INVALID;
        }
        ADC_InitStructure.ADC_Resolution = resolution;
        ADC_Init(context->adc, &ADC_InitStructure);
        break;
    default:
        return ADC_ERR_PARAMETER_INVALID;
    }

    return 0;
}


void HAL_ADC::reset(){
    if (static_cast<int>(context->idx) > static_cast<int>(STM32F4_MAX_ADC_IDX)) return;

    /* reset ADC */
    RCC_APB2PeriphResetCmd(context->RCC_APB2Periph_ADCx, ENABLE);
    RCC_APB2PeriphResetCmd(context->RCC_APB2Periph_ADCx, DISABLE);


    /* reset GPIOs */
    GPIO_InitTypeDef gpioInit;
    GPIO_StructInit(&gpioInit); // set default values in struct

    ADCChannelProps props;
    for(uint32_t i=0;i<ADC_CH_016;i++){
        if(context->channelsInUse[i] != -1){
            props = context->getChannelProperties((ADC_CHANNEL)context->channelsInUse[i]);
            gpioInit.GPIO_Pin = props.pin;
            GPIO_Init(props.port, &gpioInit);
            context->channelsInUse[i] = -1;
        }
    }
}


/* ADC is running @ APB2/2 = 84MHz/2 = 42MHz
 * - with current settings one conversion takes about 12+15 cycles = 0.642us
 */
uint16_t HAL_ADC::read(ADC_CHANNEL channel) {
    if (static_cast<int>(context->idx) > static_cast<int>(STM32F4_MAX_ADC_IDX)) return ADC_ERR_INVALID_INDEX;
    if ( (static_cast<int>(channel) > static_cast<int>(STM32F4_MAX_ADC_CHAN))) return ADC_ERR_INVALID_CHANNEL;
    if(!context->initialized) return ADC_ERR_NO_INIT;

    ADC_Cmd(context->adc, ENABLE);  // Enable ADC
    ADC_RegularChannelConfig(context->adc, channel, 1, ADC_SampleTime_15Cycles);
    ADC_SoftwareStartConv(context->adc);  //Start Conversion

    int64_t startTime = NOW();
    while (ADC_GetFlagStatus(context->adc,ADC_FLAG_EOC) == RESET){
        if ((NOW() - startTime) > 1*MILLISECONDS) {
            ADC_Cmd(context->adc, DISABLE);
            this->upCallReadFinished();
            return ADC_ERR_CONV_FAIL;
        }
    }

    uint16_t adcVal = ADC_GetConversionValue(context->adc);
    ADC_Cmd(context->adc, DISABLE);

    this->upCallReadFinished();

    return adcVal;
}







/* HW_HAL_ADC */
ADCChannelProps HW_HAL_ADC::getChannelProperties(ADC_CHANNEL channel){
    ADCChannelProps props;

    if( (idx == ADC_IDX1) || (idx == ADC_IDX2) ){
        switch(channel){
        case ADC_CH_000: props.port = GPIOA; props.pin = GPIO_Pin_0; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_001: props.port = GPIOA; props.pin = GPIO_Pin_1; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_002: props.port = GPIOA; props.pin = GPIO_Pin_2; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_003: props.port = GPIOA; props.pin = GPIO_Pin_3; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_004: props.port = GPIOA; props.pin = GPIO_Pin_4; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_005: props.port = GPIOA; props.pin = GPIO_Pin_5; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_006: props.port = GPIOA; props.pin = GPIO_Pin_6; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_007: props.port = GPIOA; props.pin = GPIO_Pin_7; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_008: props.port = GPIOB; props.pin = GPIO_Pin_0; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOB; break;
        case ADC_CH_009: props.port = GPIOB; props.pin = GPIO_Pin_1; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOB; break;
        case ADC_CH_010: props.port = GPIOC; props.pin = GPIO_Pin_0; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_011: props.port = GPIOC; props.pin = GPIO_Pin_1; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_012: props.port = GPIOC; props.pin = GPIO_Pin_2; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_013: props.port = GPIOC; props.pin = GPIO_Pin_3; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_014: props.port = GPIOC; props.pin = GPIO_Pin_4; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_015: props.port = GPIOC; props.pin = GPIO_Pin_5; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        default: break;
        }
    }

    if(idx == ADC_IDX3){
        switch(channel){
        case ADC_CH_000: props.port = GPIOA; props.pin = GPIO_Pin_0;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_001: props.port = GPIOA; props.pin = GPIO_Pin_1;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_002: props.port = GPIOA; props.pin = GPIO_Pin_2;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_003: props.port = GPIOA; props.pin = GPIO_Pin_3;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOA; break;
        case ADC_CH_004: props.port = GPIOF; props.pin = GPIO_Pin_6;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        case ADC_CH_005: props.port = GPIOF; props.pin = GPIO_Pin_7;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        case ADC_CH_006: props.port = GPIOF; props.pin = GPIO_Pin_8;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        case ADC_CH_007: props.port = GPIOF; props.pin = GPIO_Pin_9;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        case ADC_CH_008: props.port = GPIOF; props.pin = GPIO_Pin_10; props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        case ADC_CH_009: props.port = GPIOF; props.pin = GPIO_Pin_3;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        case ADC_CH_010: props.port = GPIOC; props.pin = GPIO_Pin_0;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_011: props.port = GPIOC; props.pin = GPIO_Pin_1;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_012: props.port = GPIOC; props.pin = GPIO_Pin_2;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_013: props.port = GPIOC; props.pin = GPIO_Pin_3;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOC; break;
        case ADC_CH_014: props.port = GPIOF; props.pin = GPIO_Pin_4;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        case ADC_CH_015: props.port = GPIOF; props.pin = GPIO_Pin_5;  props.RCC_AHB1Periph_GPIOx = RCC_AHB1Periph_GPIOF; break;
        default: break;
        }
    }

    return props;
}

#ifndef NO_RODOS_NAMESPACE
}
#endif

