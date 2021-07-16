/*
 * hal_pwm.h
 *
 *  Created on: 21.06.2012
 *      Author: Michael Ruffer, Thomas Walter
 */
#pragma once

#include "hal/genericIO.h"


namespace RODOS {

enum PWM_PARAMETER_TYPE {
    PWM_PARAMETER_FREQUENCY,
    PWM_PARAMETER_INCREMENTS
};

enum PWM_IDX {     // STM32F4                  AVR32
    PWM_IDX00 = 0, // Timer 1 CH1 - PE9        PWM0 -
    PWM_IDX01,     // Timer 1 CH2 - PE11       PWM1 -
    PWM_IDX02,     // Timer 1 CH3 - PE13       PWM2 - Pin PB21
    PWM_IDX03,     // Timer 1 CH4 - PE14       PWM3 - Pin PB22
    PWM_IDX04,     // Timer 2 CH1 - PA0        PWM4 -
    PWM_IDX05,     // Timer 2 CH2 - PA1        PWM5 -
    PWM_IDX06,     // Timer 2 CH3 - PA2        PWM6 -
    PWM_IDX07,     // Timer 2 CH4 - PA3
    PWM_IDX08,     // Timer 3 CH1 - PA6
    PWM_IDX09,     // Timer 3 CH2 - PC7
    PWM_IDX10,     // Timer 3 CH3 - PB0
    PWM_IDX11,     // Timer 3 CH4 - PB1
    PWM_IDX12,     // Timer 4 CH1 - PD12
    PWM_IDX13,     // Timer 4 CH2 - PD13
    PWM_IDX14,     // Timer 4 CH3 - PD14
    PWM_IDX15      // Timer 4 CH4 - PD15
};


class HW_HAL_PWM;

class HAL_PWM : public GenericIOInterface { ///< one object per pwm

    HW_HAL_PWM* context;

  public:
    HAL_PWM(){};
    HAL_PWM(PWM_IDX idx);
	void setIdx(PWM_IDX idx);
    
    /**
	 * Initialize PWM without starting it -> start with "write(N)"
	 * To get exact PWM frequency you must choose frequency and increments so that timerclock/frequency/increments = integer (no fraction)
     * e.g. STM32F4 Timer 1: 168Mhz/1000/100 = 1680 :), but 168Mhz/1000/128 = 1315.5 -> PWM freq = 168MHz/1315/128 = 998.1Hz :(
     *
     * STM32F4  : - Tim1 (16 bit,168MHz), Tim2 (32 bit,84MHz), Tim3 (16 bit,84MHz), Tim4 (16 bit,84MHz)
     *            - PWMs of the same timer can only run with same settings
     * AVR32    : all PWMs are fixed to frequency = 125.9 Hz, increments = 512 !!!
	 *
	 * @param int frequency - frequency of PWM signal
	 * @param int increments - one PWM period is divided in N increments
	 */
    int32_t init(uint32_t frequency = 1000, uint32_t increments = 100);

    int32_t config(PWM_PARAMETER_TYPE type, int32_t paramVal = 0);

    void reset();

    bool isWriteFinished() { return true; }

    /*** Extra only for PWM **/
    /**
     * Set pulse width of PWM signal
     * - write(increments/2) -> duty cycle 50%
     * - write(0) -> stop PWM-output
     * @param int pulseWidthInIncs
     */
    int32_t write(uint32_t pulseWidthInIncs); // 0 ... +increments
};


} // namespace RODOS
