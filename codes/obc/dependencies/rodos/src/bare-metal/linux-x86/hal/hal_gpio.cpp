#include <new>
#include "rodos.h"
#include "hal/hal_gpio.h"

namespace RODOS {

class HW_HAL_GPIO {
  public:
    HW_HAL_GPIO(GPIO_PIN pinIdx, uint8_t numOfPins, bool isOutput) : pinIdx(pinIdx), numOfPins(numOfPins), isOutput(isOutput), pinsValue(0){};

    GPIO_PIN pinIdx;
    uint8_t  numOfPins;
    bool     isOutput;
    uint32_t pinsValue;
};


HAL_GPIO::HAL_GPIO(GPIO_PIN pinIdx) {
    // context = new HW_HAL_GPIO(pinIdx,1,false);
    context = (HW_HAL_GPIO*)xmalloc(sizeof(HW_HAL_GPIO)); // dynamic memory allocation with RODOS function
    RODOS_ASSERT_IFNOT_RETURN_VOID(context != nullptr);
    context = new(context) HW_HAL_GPIO(pinIdx, 1, false); // placement new to call constructor
}


int32_t HAL_GPIO::init(bool isOutput, uint8_t numOfPins, uint32_t initVal) {
    if(context == 0) return -1;
    if(numOfPins > 0) context->numOfPins = numOfPins; // numOfPins has to be > 0 -> if new value is 0 keep the default value

    context->isOutput = isOutput;

    if(context->isOutput) {
        setPins(initVal);
    }

    return 0;
}


int32_t HAL_GPIO::config(GPIO_CFG_TYPE type, uint32_t paramVal) {
    if(context == NULL) return -1;
    switch(type) {
        case GPIO_CFG_OUTPUT_ENABLE:
            if(paramVal > 0) {
                context->isOutput = true;
            } else {
                context->isOutput = false;
            }
            return 0;

        case GPIO_CFG_NUM_OF_PINS:
            if(paramVal <= UINT8_MAX) { // numOfPins has to be <= 255 (UINT8_MAX) -> uint8_t
                context->numOfPins = static_cast<uint8_t>(paramVal);
                return 0;
            }
            return -1;

        default: return -1;
    }
}


void HAL_GPIO::reset() {
    if(context == NULL) return;
    config(GPIO_CFG_OUTPUT_ENABLE, 0);
    context->pinsValue = 0;
}


void HAL_GPIO::setPins(uint32_t val) {
    if(context == 0) return;
    if(context->isOutput) {
        //PRINTF("HAL_GPIO::setPins(): 0x%08x -> 0x%08x\n", context->pinsValue, val);
        context->pinsValue = val;
    }
}


uint32_t HAL_GPIO::readPins() {
    //PRINTF("HAL_GPIO::readPins(): 0x%08x\n", context->pinsValue);
    return context->pinsValue;
}


bool HAL_GPIO::isDataReady() {
    return true;
}

} // namespace RODOS
