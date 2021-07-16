#include <new>
#include "rodos.h"
#include "hal/hal_gpio.h"
#include "hw_hal_gpio.h"

#include "stm32f4xx_gpio.h"

namespace RODOS {


void initEXTInterrupts();
void initEXTInterrupts() {
    static bool init = false;
    if(!init) {
        init = true;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        NVIC_EnableIRQ(EXTI0_IRQn);
        NVIC_EnableIRQ(EXTI1_IRQn);
        NVIC_EnableIRQ(EXTI2_IRQn);
        NVIC_EnableIRQ(EXTI3_IRQn);
        NVIC_EnableIRQ(EXTI4_IRQn);
        NVIC_EnableIRQ(EXTI9_5_IRQn);
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
}


/***** class HW_HAL_GPIO *****/
HW_HAL_GPIO* extInterruptLines[16];

HW_HAL_GPIO::HW_HAL_GPIO(GPIO_PIN pinIdx, uint8_t numOfPins, bool isOutput) : pinIdx(pinIdx), numOfPins(numOfPins), isOutput(isOutput) {
    setPinMask();
    PORT = getSTM32Port(pinIdx);

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = pinMask;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;

    irqSensitivity       = GPIO_IRQ_SENS_BOTH;
    interruptEventOcured = false;
}


void HW_HAL_GPIO::setPinMask() {
    if(numOfPins + (pinIdx & 0xF) > 16) { // pin-group exceeds port boundary ! only the pins up to most significant pin of port will be set
        pinMask = static_cast<uint16_t>(0xFFFF << (pinIdx & 0xF));
    } else {
        pinMask = static_cast<uint16_t>(0xFFFF >> (16 - numOfPins));
        pinMask = static_cast<uint16_t>(pinMask << (pinIdx & 0xF));
    }
}


void HW_HAL_GPIO::EXTIRQHandler() {
    interruptEventOcured = true;
    hal_gpio->upCallDataReady();
    EXTI->PR = 1u << (pinIdx % 16);
}


uint8_t HW_HAL_GPIO::getGPIO_PinSource(uint32_t GPIO_Pin) {

    uint8_t GPIO_PinSource = 0;

    while(GPIO_Pin >>= 1) GPIO_PinSource++;

    return GPIO_PinSource;
}


uint32_t HW_HAL_GPIO::getRCC_APB1Periph_GPIOx(GPIO_TypeDef* port) {

    switch((uint32_t)port) {
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


GPIO_TypeDef* HW_HAL_GPIO::getSTM32Port(GPIO_PIN pinIdx) {
    if(pinIdx < GPIO_016) {
        return GPIOA;
    } else if(pinIdx < GPIO_032) {
        return GPIOB;
    } else if(pinIdx < GPIO_048) {
        return GPIOC;
    } else if(pinIdx < GPIO_064) {
        return GPIOD;
    } else if(pinIdx < GPIO_080) {
        return GPIOE;
    } else if(pinIdx < GPIO_096) {
        return GPIOF;
    } else if(pinIdx < GPIO_112) {
        return GPIOG;
    } else if(pinIdx < GPIO_128) {
        return GPIOH;
    } else if(pinIdx <= GPIO_139) {
        return GPIOI;
    } else {
        return NULL;
    }
}

uint16_t HW_HAL_GPIO::getSTM32Pin(GPIO_PIN pinIdx) {
    //return 1 << (pinIdx%16);
    return static_cast<uint16_t>(1 << (pinIdx & 0xf));
}


int32_t HW_HAL_GPIO::configureAFPin(GPIO_PIN pinIdx, uint8_t GPIO_AF_XXX) {
    if(pinIdx < GPIO_000 || pinIdx > GPIO_139) return -1;
    GPIO_TypeDef* port = getSTM32Port(pinIdx);
    uint16_t      pin  = getSTM32Pin(pinIdx);

    // Enable GPIO clock and release reset
    RCC_AHB1PeriphClockCmd(getRCC_APB1Periph_GPIOx(port), ENABLE);
    RCC_AHB1PeriphResetCmd(getRCC_APB1Periph_GPIOx(port), DISABLE);

    // configure pin multiplexer -> choose alternate function (AF) UART
    GPIO_PinAFConfig(port, getGPIO_PinSource(pin), GPIO_AF_XXX);

    // configure pin
    GPIO_InitTypeDef Gis;
    GPIO_StructInit(&Gis);
    Gis.GPIO_Mode = GPIO_Mode_AF;
    Gis.GPIO_Pin  = pin;
    GPIO_Init(port, &Gis);

    return 0;
}


int32_t HW_HAL_GPIO::resetPin(GPIO_PIN pinIdx) {
    if(pinIdx < GPIO_000 || pinIdx > GPIO_139) return -1;

    GPIO_InitTypeDef Gis;
    GPIO_StructInit(&Gis);
    Gis.GPIO_Mode = GPIO_Mode_IN;

    Gis.GPIO_Pin = getSTM32Pin(pinIdx);
    GPIO_Init(getSTM32Port(pinIdx), &Gis);

    return 0;
}
/***** END class HW_HAL_GPIO *****/


HAL_GPIO::HAL_GPIO(GPIO_PIN pinIdx) {
    //context = new HW_HAL_GPIO(pinIdx,1,false);
    context           = (HW_HAL_GPIO*)xmalloc(sizeof(HW_HAL_GPIO)); // dynamic memory allocation with RODOS function
    context           = new(context) HW_HAL_GPIO(pinIdx, 1, false); // placement new to call constructor
    context->hal_gpio = this;
}

void HAL_GPIO::setPinNumber(GPIO_PIN pinIdx) {
    //context = new HW_HAL_GPIO(pinIdx,1,false);
    context           = (HW_HAL_GPIO*)xmalloc(sizeof(HW_HAL_GPIO)); // dynamic memory allocation with RODOS function
    context           = new(context) HW_HAL_GPIO(pinIdx, 1, false); // placement new to call constructor
    context->hal_gpio = this;
}


int32_t HAL_GPIO::init(bool isOutput, uint8_t numOfPins, uint32_t initVal) {
    if(numOfPins > 0) context->numOfPins = numOfPins; // numOfPins has to be > 0 -> if new value is 0 keep the default value

    context->isOutput = isOutput;
    context->setPinMask();

    if(context->PORT == NULL) return -1;

    RCC_AHB1PeriphClockCmd(HW_HAL_GPIO::getRCC_APB1Periph_GPIOx(context->PORT), ENABLE);

    if(context->isOutput) {
        config(GPIO_CFG_OUTPUT_ENABLE, 1);
        setPins(initVal);
    } else {
        config(GPIO_CFG_OUTPUT_ENABLE, 0);
    }

    initEXTInterrupts();
    return 0;
}


int32_t HAL_GPIO::config(GPIO_CFG_TYPE type, uint32_t paramVal) {
    switch(type) {
        case GPIO_CFG_OUTPUT_ENABLE:
            if(paramVal > 0) {
                context->isOutput                  = true;
                context->GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
            } else {
                context->isOutput                  = false;
                context->GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
            }
            GPIO_Init(context->PORT, &context->GPIO_InitStruct);
            return 0;

        case GPIO_CFG_NUM_OF_PINS:
            if(paramVal > 0 && paramVal < 256) { // numOfPins has to be > 0 and < 256 -> uint8_t
                context->numOfPins = static_cast<uint8_t>(paramVal);
                context->setPinMask();
                context->GPIO_InitStruct.GPIO_Pin = context->pinMask;
                GPIO_Init(context->PORT, &context->GPIO_InitStruct);
                return 0;
            }
            return -1;

        case GPIO_CFG_PULLUP_ENABLE:
            if(paramVal > 0) {
                if(paramVal) {
                    context->GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
                } else {
                    context->GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
                }
                GPIO_Init(context->PORT, &context->GPIO_InitStruct);
                return 0;
            }
            return -1;

        case GPIO_CFG_PULLDOWN_ENABLE:
            if(paramVal > 0) {
                if(paramVal) {
                    context->GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
                } else {
                    context->GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
                }
                GPIO_Init(context->PORT, &context->GPIO_InitStruct);
                return 0;
            }
            return -1;
        case GPIO_CFG_IRQ_SENSITIVITY:
            if(paramVal <= GPIO_IRQ_SENS_FALLING) {
                context->irqSensitivity = (RODOS::GPIO_IRQ_SENSITIVITY)paramVal;
                return 0;
            }
            return -1;

        default: return -1;
    }
}


void HAL_GPIO::reset() {
    interruptEnable(false);
    //    config(GPIO_CFG_OUTPUT_ENABLE, 0);                                                  already done in resetPin()
    //    GPIO_DeInit(context->PORT);                                                         !!! we can not disable the whole port
    //    RCC_AHB1PeriphClockCmd(context->getRCC_APB1Periph_GPIOx(context->PORT), DISABLE);   !!! we can not disable the whole port
    HW_HAL_GPIO::resetPin(context->pinIdx);
}


void HAL_GPIO::setPins(uint32_t val) {
    uint16_t newPinVal = 0;

    if(context->isOutput) {
        PRIORITY_CEILER_IN_SCOPE();
        //read the whole port, change only the selected pins and write the value
        newPinVal = static_cast<uint16_t>(GPIO_ReadOutputData(context->PORT) & ~context->pinMask);             // get current pinvalues of whole port and clear pinvalues we want to set new
        newPinVal = static_cast<uint16_t>(newPinVal | ((val << (context->pinIdx & 0x0F)) & context->pinMask)); // set new pinvalues
        GPIO_Write(context->PORT, newPinVal);
    }
}


uint32_t HAL_GPIO::readPins() {
    if(context->isOutput) {
        return static_cast<uint32_t>(GPIO_ReadOutputData(context->PORT) & context->pinMask) >> (context->pinIdx & 0xF);
    }
    return static_cast<uint32_t>(GPIO_ReadInputData(context->PORT) & context->pinMask) >> (context->pinIdx & 0xF);
}


bool HAL_GPIO::isDataReady() {
    return context->interruptEventOcured;
}

//When rising and/or falling edge occures dataReady() == true
void HAL_GPIO::interruptEnable(bool enable) {
    if(context->pinIdx == GPIO_INVALID) { return; }

    uint8_t portNum = static_cast<uint8_t>(context->pinIdx / 16);
    uint8_t pinNum  = static_cast<uint8_t>(context->pinIdx % 16);
    int32_t exti    = pinNum;

    if(enable) { //enable Interrupt
        if(extInterruptLines[exti] == context) {
            return; //Already enabled
        } else if(extInterruptLines[exti] == 0) {
            RODOS_ASSERT_IFNOT_RETURN_VOID(context->numOfPins <= 1); // IRQ not possible with numOfPins > 1

            SYSCFG_EXTILineConfig(portNum, pinNum);

            if(context->irqSensitivity == GPIO_IRQ_SENS_RISING || context->irqSensitivity == GPIO_IRQ_SENS_BOTH) {
                EXTI->RTSR |= 1lu << pinNum;
            }
            if(context->irqSensitivity == GPIO_IRQ_SENS_FALLING || context->irqSensitivity == GPIO_IRQ_SENS_BOTH) {
                EXTI->FTSR |= 1lu << pinNum;
            }

            extInterruptLines[exti]       = context;
            context->interruptEventOcured = false;
            EXTI->IMR |= 1lu << pinNum;

        } else {
            RODOS_ERROR("External IRQ Line already used by another HAL_GPIO");
            return;
        }


    } else { //disable Interrupt
        if(extInterruptLines[exti] == context) {
            EXTI->IMR &= ~(1lu << pinNum);
            EXTI->RTSR &= ~(1lu << pinNum);
            EXTI->FTSR &= ~(1lu << pinNum);
            extInterruptLines[exti] = 0;
        }
    }
}

// dataReady == false
void HAL_GPIO::resetInterruptEventStatus() {
    context->interruptEventOcured = false;
}


} // namespace RODOS


extern "C" {

#ifndef RODOS_UNMOUNT_IRQ_EXTI0
void EXTI0_IRQHandler();
void EXTI0_IRQHandler() {
    if(extInterruptLines[0]) {
        extInterruptLines[0]->EXTIRQHandler();
    }
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_EXTI1
void EXTI1_IRQHandler();
void EXTI1_IRQHandler() {
    if(extInterruptLines[1]) {
        extInterruptLines[1]->EXTIRQHandler();
    }
    NVIC_ClearPendingIRQ(EXTI1_IRQn);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_EXTI2
void EXTI2_IRQHandler();
void EXTI2_IRQHandler() {
    if(extInterruptLines[2]) {
        extInterruptLines[2]->EXTIRQHandler();
    }
    NVIC_ClearPendingIRQ(EXTI2_IRQn);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_EXTI3
void EXTI3_IRQHandler();
void EXTI3_IRQHandler() {
    if(extInterruptLines[3]) {
        extInterruptLines[3]->EXTIRQHandler();
    }
    NVIC_ClearPendingIRQ(EXTI3_IRQn);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_EXTI4
void EXTI4_IRQHandler();
void EXTI4_IRQHandler() {
    if(extInterruptLines[4]) {
        extInterruptLines[4]->EXTIRQHandler();
    }
    NVIC_ClearPendingIRQ(EXTI4_IRQn);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_EXTI9_5
void EXTI9_5_IRQHandler();
void EXTI9_5_IRQHandler() {
    uint32_t pending = EXTI->PR;
    for(int i = 5; i <= 9; i++) {
        if(((pending >> i) & 0x01) && extInterruptLines[i]) {
            extInterruptLines[i]->EXTIRQHandler();
        }
    }
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_EXTI15_10
void EXTI15_10_IRQHandler();
void EXTI15_10_IRQHandler() {
    uint32_t pending = EXTI->PR;
    for(int i = 10; i <= 15; i++) {
        if(((pending >> i) & 0x01) && extInterruptLines[i]) {
            extInterruptLines[i]->EXTIRQHandler();
        }
    }
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
}
#endif
}
