// Rishav (2021)

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "rodos.h"

#define MOTOR_PWM_FREQUENCY 8000UL
#define MOTOR_PWM_MAX_DUTY_CYCLE 100UL

/* Instantiation for four H-Bridges */
// Motor motor_a(PWM_IDX12, GPIO_036,  GPIO_017); // A: PD12, PC04, IN2:PB1
// Motor motor_b(PWM_IDX13, GPIO_016,  GPIO_071); // B: PD13, PB00, IN2:PE7
// Motor motor_c(PWM_IDX14, GPIO_072,  GPIO_074); // C: PD14, PE08, IN2:PE10
// Motor motor_d(PWM_IDX15, GPIO_076,  GPIO_079); // D: PD15, PE12, IN2:PE15

class Motor
{

public:
    Motor(PWM_IDX pwm_idx_, GPIO_PIN dir1_idx, GPIO_PIN dir2_idx);
    void init();
    void setDutycycle(float dutycycle);

private:
    HAL_PWM pwm;
    HAL_GPIO dir1;
    HAL_GPIO dir2;
};

Motor::Motor(PWM_IDX pwm_idx, GPIO_PIN dir1_pin, GPIO_PIN dir2_pin)
{
    // Instantiate PWM and GPIO objects
    pwm.setIdx(pwm_idx);
    dir1.setPinNumber(dir1_pin);
    dir2.setPinNumber(dir2_pin);
}

void Motor::init()
{
    // Init PWM
    pwm.init(8000, 100);

    // Init direction pins
    dir1.init(true, 1, 0);
    dir2.init(true, 1, 0);
}

void Motor::setDutycycle(float dutycycle)
{
    dutycycle = dutycycle * (MOTOR_PWM_MAX_DUTY_CYCLE / 100.0F);

    if (dutycycle < 0)
    {
        dutycycle = -dutycycle;
        dir1.setPins(0);
        dir2.setPins(1);
    }
    else if (dutycycle >= 0)
    {
        dir1.setPins(1);
        dir2.setPins(0);
    }
    pwm.write(dutycycle);
}

#endif // motor.h