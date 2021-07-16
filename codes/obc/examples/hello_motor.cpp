// Using PWM to change brightness of onboard LEDs

#include "rodos.h"
#include "motor.h"

HAL_GPIO pin_dcdc_en(GPIO_066); // PE02

/* Use one of these to instantiate object for H-Bridge A, B, C or D */
// Motor rw(PWM_IDX12, GPIO_036,  GPIO_017); // A: PD12, PC04, IN2:PB1
// Motor rw(PWM_IDX13, GPIO_016,  GPIO_071); // B: PD13, PB00, IN2:PE7
// Motor rw(PWM_IDX14, GPIO_072,  GPIO_074); // C: PD14, PE08, IN2:PE10
Motor rw(PWM_IDX15, GPIO_076, GPIO_079); // D: PD15, PE12, IN2:PE15

class LedPwm : public StaticThread<>
{
public:
    uint8_t duty_cycle = 0;

    void init()
    {
        pin_dcdc_en.init(true, 1, 1);
        rw.init();

        PRINTF("Motor Initialized !!\r\n");
    }

    void run()
    {
        init();
        rw.setDutycycle(10);
    }
} led_pwm;