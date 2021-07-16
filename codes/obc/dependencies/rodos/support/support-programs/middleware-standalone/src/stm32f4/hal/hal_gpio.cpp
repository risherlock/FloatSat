#include <new>

#include "hal/hal_gpio.h"
#include "hw_hal_gpio.h"

#include "stm32f4xx_gpio.h"

#define NULL 0

#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif




/***** class HW_HAL_GPIO *****/


uint32_t HW_HAL_GPIO::getGPIO_PinSource(uint32_t GPIO_Pin) {

    uint32_t GPIO_PinSource = 0;

    while (GPIO_Pin >>= 1) GPIO_PinSource++;

    return GPIO_PinSource;
}


uint32_t HW_HAL_GPIO::getRCC_APB1Periph_GPIOx(GPIO_TypeDef *port) {

    switch((uint32_t)port){
    case GPIOA_BASE: return RCC_AHB1Periph_GPIOA;
    case GPIOB_BASE: return RCC_AHB1Periph_GPIOB;
    case GPIOC_BASE: return RCC_AHB1Periph_GPIOC;
    case GPIOD_BASE: return RCC_AHB1Periph_GPIOD;
    case GPIOE_BASE: return RCC_AHB1Periph_GPIOE;
    case GPIOF_BASE: return RCC_AHB1Periph_GPIOF;
    case GPIOG_BASE: return RCC_AHB1Periph_GPIOG;
    case GPIOH_BASE: return RCC_AHB1Periph_GPIOH;
    case GPIOI_BASE: return RCC_AHB1Periph_GPIOI;
    case GPIOJ_BASE: return RCC_AHB1Periph_GPIOJ;
    case GPIOK_BASE: return RCC_AHB1Periph_GPIOK;
    default: return 0;
    }
}


GPIO_TypeDef* HW_HAL_GPIO::getSTM32Port(GPIO_PIN pinIdx){
    if      (pinIdx < GPIO_016)   {   return GPIOA; }
    else if (pinIdx < GPIO_032)   {   return GPIOB; }
    else if (pinIdx < GPIO_048)   {   return GPIOC; }
    else if (pinIdx < GPIO_064)   {   return GPIOD; }
    else if (pinIdx < GPIO_080)   {   return GPIOE; }
    else if (pinIdx < GPIO_096)   {   return GPIOF; }
    else if (pinIdx < GPIO_112)   {   return GPIOG; }
    else if (pinIdx < GPIO_128)   {   return GPIOH; }
    else if (pinIdx <= GPIO_139)  {   return GPIOI; }
    else                    {   return NULL;  }
}

uint16_t HW_HAL_GPIO::getSTM32Pin(GPIO_PIN pinIdx){
    //return 1 << (pinIdx%16);
    return 1 << (pinIdx & 0xf);
}


int32_t HW_HAL_GPIO::configureAFPin(GPIO_PIN pinIdx, uint8_t GPIO_AF_XXX){
    if (pinIdx < GPIO_000 || pinIdx > GPIO_139) return -1;
    GPIO_TypeDef* port = getSTM32Port(pinIdx);
    uint16_t pin = getSTM32Pin(pinIdx);

    // Enable GPIO clock and release reset
    RCC_AHB1PeriphClockCmd(getRCC_APB1Periph_GPIOx(port), ENABLE);
    RCC_AHB1PeriphResetCmd(getRCC_APB1Periph_GPIOx(port), DISABLE);

    // configure pin multiplexer -> choose alternate function (AF) UART
    GPIO_PinAFConfig(port,getGPIO_PinSource(pin),GPIO_AF_XXX);

    // configure pin
    GPIO_InitTypeDef Gis;
    GPIO_StructInit(&Gis);
    Gis.GPIO_Mode = GPIO_Mode_AF;
    Gis.GPIO_Pin = pin;
    GPIO_Init(port, &Gis);

    return 0;
}


int32_t HW_HAL_GPIO::resetPin(GPIO_PIN pinIdx){
    if (pinIdx < GPIO_000 || pinIdx > GPIO_139) return -1;

    GPIO_InitTypeDef Gis;
    GPIO_StructInit(&Gis);
    Gis.GPIO_Mode = GPIO_Mode_IN;

    Gis.GPIO_Pin = getSTM32Pin(pinIdx);
    GPIO_Init(getSTM32Port(pinIdx), &Gis);

    return 0;
}
/***** END class HW_HAL_GPIO *****/




#ifndef NO_RODOS_NAMESPACE
}
#endif

