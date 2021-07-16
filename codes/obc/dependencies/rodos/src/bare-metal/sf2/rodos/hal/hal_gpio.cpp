#include <new>
#include <rodos.h>
#include <hal/hal_gpio.h>
#include "hw_hal_gpio.h"

namespace RODOS {

/***** class HW_HAL_GPIO *****/
HW_HAL_GPIO* extInterruptLines[32] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
bool mssInit = false;

HW_HAL_GPIO::HW_HAL_GPIO(GPIO_PIN pinIdx, uint8_t numOfPins, bool isOutput) : pinIdx(pinIdx), numOfPins(numOfPins), isOutput(isOutput) {
    setPinMask();
    irqSensitivity       = GPIO_IRQ_SENS_BOTH;
    interruptEventOcured = false;
}

void HW_HAL_GPIO::setPinMask(void) {
    if(numOfPins + (pinIdx & 0x1F) > 32) { // pin-group exceeds port boundary ! only the pins up to most significant pin of port will be set
        pinMask = 0xFFFFFFFF << (pinIdx & 0x1F);
    } else {
        pinMask = 0xFFFFFFFF >> (32 - numOfPins);
        pinMask = pinMask << (pinIdx & 0x1F);
    }
}

void HW_HAL_GPIO::EXTIRQHandler(void) {
    interruptEventOcured = true;
    hal_gpio->upCallDataReady();
    MSS_GPIO_clear_irq((mss_gpio_id_t)pinIdx);
}

/***** END class HW_HAL_GPIO *****/


HAL_GPIO::HAL_GPIO(GPIO_PIN pinIdx) {
    //context = new HW_HAL_GPIO(pinIdx,1,false);
    context           = (HW_HAL_GPIO*)xmalloc(sizeof(HW_HAL_GPIO)); // dynamic memory allocation with RODOS function
    context           = new(context) HW_HAL_GPIO(pinIdx, 1, false); // placement new to call constructor
    context->hal_gpio = this;
}


int32_t HAL_GPIO::init(bool isOutput, uint8_t numOfPins, uint32_t initVal) {
    if(numOfPins > 0) {
        context->numOfPins = numOfPins; // numOfPins has to be > 0 -> if new value is 0 keep the default value
    }
    context->isOutput = isOutput;
    if(!mssInit) {       // init only once
        MSS_GPIO_init(); // deletes configuration
        mssInit = true;
    }
    if(context->isOutput) {
        config(GPIO_CFG_OUTPUT_ENABLE, 1);
        setPins(initVal);
    } else {
        config(GPIO_CFG_OUTPUT_ENABLE, 0);
    }
    return 0;
}


int32_t HAL_GPIO::config(GPIO_CFG_TYPE type, uint32_t paramVal) {
    switch(type) {
        case GPIO_CFG_OUTPUT_ENABLE:
            if(paramVal > 0) {
                context->isOutput = true;
                MSS_GPIO_config((mss_gpio_id_t)context->pinIdx, MSS_GPIO_OUTPUT_MODE);
            } else {
                context->isOutput = false;
                MSS_GPIO_config((mss_gpio_id_t)context->pinIdx, MSS_GPIO_INPUT_MODE);
            }
            return 0;

        case GPIO_CFG_NUM_OF_PINS:
            if((paramVal > 0) && (paramVal < 32)) { // numOfPins has to be > 0 and < 256 -> uint8_t
                context->numOfPins = static_cast<uint8_t>(paramVal);
                context->setPinMask();
                return 0;
            }
            return -1;

        case GPIO_CFG_PULLUP_ENABLE:
            RODOS_ERROR("Pull Up for GPIOs not available on SF2");
            return -1;

        case GPIO_CFG_PULLDOWN_ENABLE:
            RODOS_ERROR("Pull Down for GPIOs not available on SF2");
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


void HAL_GPIO::reset() { // Was soll reset machen ??
    interruptEnable(false);
    MSS_GPIO_config((mss_gpio_id_t)context->pinIdx, MSS_GPIO_INPUT_MODE);
    //HW_HAL_GPIO::resetPin(context->pinIdx);
}


void HAL_GPIO::setPins(uint32_t val) {
    uint32_t newPinVal = 0;
    if(context->isOutput) {
        PRIORITY_CEILER_IN_SCOPE();
        //read the whole port, change only the selected pins and write the value
        newPinVal = MSS_GPIO_get_outputs() & ~context->pinMask;            // get current pinvalues of whole port and clear pinvalues we want to set new
        newPinVal |= (val << (context->pinIdx & 0x1F)) & context->pinMask; // set new pinvalues
        MSS_GPIO_set_outputs(newPinVal);
    }
}


uint32_t HAL_GPIO::readPins() {
    if(context->isOutput) {
        return (MSS_GPIO_get_outputs() & context->pinMask) >> (context->pinIdx & 0x1F);
    }
    return (MSS_GPIO_get_inputs() & context->pinMask) >> (context->pinIdx & 0x1F);
}


bool HAL_GPIO::isDataReady() {
    return context->interruptEventOcured;
}


//When rising and/or falling edge occures dataReady() == true
void HAL_GPIO::interruptEnable(bool enable) {
    if(context->pinIdx == GPIO_INVALID) {
        return;
    }
    mss_gpio_id_t exti = (mss_gpio_id_t)context->pinIdx;
    if(enable) { //enable Interrupt
        if(extInterruptLines[exti] == context) {
            return; //Already enabled
        } else if(extInterruptLines[exti] == 0) {
            if(context->numOfPins > 1) {
                RODOS_ERROR("IRQ not possible with numOfPins > 1");
                return;
            }
            uint32_t config = MSS_GPIO_INPUT_MODE;
            if(context->irqSensitivity == GPIO_IRQ_SENS_RISING) {
                config |= MSS_GPIO_IRQ_EDGE_POSITIVE;
            } else if(context->irqSensitivity == GPIO_IRQ_SENS_FALLING) {
                config |= MSS_GPIO_IRQ_EDGE_POSITIVE;
            } else if(context->irqSensitivity == GPIO_IRQ_SENS_BOTH) {
                config |= MSS_GPIO_IRQ_EDGE_BOTH;
            }
            extInterruptLines[exti]       = context;
            context->interruptEventOcured = false;
            MSS_GPIO_config(exti, config);
            MSS_GPIO_enable_irq(exti);
        } else {
            RODOS_ERROR("External IRQ Line already used by another HAL_GPIO");
            return;
        }
    } else { //disable Interrupt
        if(extInterruptLines[exti] == context) {
            MSS_GPIO_disable_irq(exti);
            extInterruptLines[exti] = 0;
        }
    }
}

// dataReady == false
void HAL_GPIO::resetInterruptEventStatus() {
    context->interruptEventOcured = false;
}

} /* namespace RODOS */

#define interruptFunc __attribute__((__interrupt__))

extern "C" {

#ifndef RODOS_UNMOUNT_IRQ_GPIO0
interruptFunc void GPIO0_IRQHandler(void) {
    if(extInterruptLines[0]) {
        extInterruptLines[0]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_0);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO1
interruptFunc void GPIO1_IRQHandler(void) {
    if(extInterruptLines[1]) {
        extInterruptLines[1]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_1);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO2
interruptFunc void GPIO2_IRQHandler(void) {
    if(extInterruptLines[2]) {
        extInterruptLines[2]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_2);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO3
interruptFunc void GPIO3_IRQHandler(void) {
    if(extInterruptLines[3]) {
        extInterruptLines[3]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_3);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO4
interruptFunc void GPIO4_IRQHandler(void) {
    if(extInterruptLines[4]) {
        extInterruptLines[4]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_4);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO5
interruptFunc void GPIO5_IRQHandler(void) {
    if(extInterruptLines[5]) {
        extInterruptLines[5]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_5);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO6
interruptFunc void GPIO6_IRQHandler(void) {
    if(extInterruptLines[6]) {
        extInterruptLines[6]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_6);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO7
interruptFunc void GPIO7_IRQHandler(void) {
    if(extInterruptLines[7]) {
        extInterruptLines[7]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_7);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO8
interruptFunc void GPIO8_IRQHandler(void) {
    if(extInterruptLines[8]) {
        extInterruptLines[8]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_8);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO9
interruptFunc void GPIO9_IRQHandler(void) {
    if(extInterruptLines[9]) {
        extInterruptLines[9]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_9);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO10
interruptFunc void GPIO10_IRQHandler(void) {
    if(extInterruptLines[10]) {
        extInterruptLines[10]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_10);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO11
interruptFunc void GPIO11_IRQHandler(void) {
    if(extInterruptLines[11]) {
        extInterruptLines[11]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_11);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO12
interruptFunc void GPIO12_IRQHandler(void) {
    if(extInterruptLines[12]) {
        extInterruptLines[12]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_12);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO13
interruptFunc void GPIO13_IRQHandler(void) {
    if(extInterruptLines[13]) {
        extInterruptLines[13]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_13);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO14
interruptFunc void GPIO14_IRQHandler(void) {
    if(extInterruptLines[14]) {
        extInterruptLines[14]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_14);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO15
interruptFunc void GPIO15_IRQHandler(void) {
    if(extInterruptLines[15]) {
        extInterruptLines[15]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_15);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO16
interruptFunc void GPIO16_IRQHandler(void) {
    if(extInterruptLines[16]) {
        extInterruptLines[16]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_16);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO17
interruptFunc void GPIO17_IRQHandler(void) {
    if(extInterruptLines[17]) {
        extInterruptLines[17]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_17);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO18
interruptFunc void GPIO18_IRQHandler(void) {
    if(extInterruptLines[18]) {
        extInterruptLines[18]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_18);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO19
interruptFunc void GPIO19_IRQHandler(void) {
    if(extInterruptLines[19]) {
        extInterruptLines[19]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_19);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO20
interruptFunc void GPIO20_IRQHandler(void) {
    if(extInterruptLines[20]) {
        extInterruptLines[20]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_20);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO21
interruptFunc void GPIO21_IRQHandler(void) {
    if(extInterruptLines[21]) {
        extInterruptLines[21]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_21);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO22
interruptFunc void GPIO22_IRQHandler(void) {
    if(extInterruptLines[22]) {
        extInterruptLines[22]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_22);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO23
interruptFunc void GPIO23_IRQHandler(void) {
    if(extInterruptLines[23]) {
        extInterruptLines[23]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_23);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO24
interruptFunc void GPIO24_IRQHandler(void) {
    if(extInterruptLines[24]) {
        extInterruptLines[24]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_24);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO25
interruptFunc void GPIO25_IRQHandler(void) {
    if(extInterruptLines[25]) {
        extInterruptLines[25]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_25);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO26
interruptFunc void GPIO26_IRQHandler(void) {
    if(extInterruptLines[26]) {
        extInterruptLines[26]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_26);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO27
interruptFunc void GPIO27_IRQHandler(void) {
    if(extInterruptLines[27]) {
        extInterruptLines[27]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_27);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO28
interruptFunc void GPIO28_IRQHandler(void) {
    if(extInterruptLines[28]) {
        extInterruptLines[28]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_28);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO29
interruptFunc void GPIO29_IRQHandler(void) {
    if(extInterruptLines[29]) {
        extInterruptLines[29]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_29);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO30
interruptFunc void GPIO30_IRQHandler(void) {
    if(extInterruptLines[30]) {
        extInterruptLines[30]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_30);
}
#endif

#ifndef RODOS_UNMOUNT_IRQ_GPIO31
interruptFunc void GPIO31_IRQHandler(void) {
    if(extInterruptLines[31]) {
        extInterruptLines[31]->EXTIRQHandler();
    }
    MSS_GPIO_clear_irq(MSS_GPIO_31);
}
#endif
}
