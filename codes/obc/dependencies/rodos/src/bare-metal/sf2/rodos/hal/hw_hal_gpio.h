#ifndef _HW_HAL_GPIO_H_
#define _HW_HAL_GPIO_H_

#include "hal/hal_gpio.h"

#include "../../drivers/mss_gpio.h"

namespace RODOS {

/** 
 *  mss_gpio_id_t   GPIO 0 bis 31 als enum 
 *  MSS_GPIO_config(MSS_GPIO_x, in/out/inout, irq level/edge);
 *  MSS_GPIO_set_output()/s();
 *  MSS_GPIO_get_input()/s()
 *  MSS_GPIO_enable_irq(MSS_GPIO_x) MSS_GPIO_disable_irq(MSS_GPIO_x)
 *  MSS_GPIO_clear_irq()
 *
 * void GPIO9_IRQHandler( void )
 * {
 *    do_interrupt_processing();     
 *    MSS_GPIO_clear_irq( MSS_GPIO_9 );
 * }
 *
 * inout MSS_GPIO_drive_inout    High Low Z
 */
class HW_HAL_GPIO {
    friend class HAL_GPIO;

protected: //private?
    GPIO_PIN             pinIdx;
    HAL_GPIO*            hal_gpio;
    uint8_t              numOfPins;
    bool                 isOutput;
    GPIO_IRQ_SENSITIVITY irqSensitivity;
    bool                 interruptEventOcured;
    uint32_t             pinMask;

    HW_HAL_GPIO(GPIO_PIN pinIdx, uint8_t numOfPins, bool isOutput);


    void setPinMask();

public:
    void EXTIRQHandler(void);
};

} /* namespace RODOS */

#endif // _HW_HAL_GPIO_H_
