/*
 * Copyright (c) 2012 Uni Wuerzburg, Wuerzburg
 * All rights reserved.
 *
 * @author Sergio Montenegro, Michael Ruffer
 */


#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_


#include "genericIO.h"
#include <stdint.h>



#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif

typedef enum {					// STM32F4		// AVR32 UC3A0512                   // NEXYS3-LEON3
    // Port A
	GPIO_000 = 0, 				// PA00			// AVR32_PIN_PA00,                  // 7SEG_DISPLAY_4_A
	GPIO_001,					// PA01         // AVR32_PIN_PA01,                  // 7SEG_DISPLAY_4_B
    GPIO_002, 					// PA02         // AVR32_PIN_PA02,                  // 7SEG_DISPLAY_4_C
    GPIO_003, 					// PA03         // AVR32_PIN_PA03,                  // 7SEG_DISPLAY_4_D
    GPIO_004, 					// PA04         // AVR32_PIN_PA04,                  // 7SEG_DISPLAY_4_E
    GPIO_005, 					// PA05         // AVR32_PIN_PA05,                  // 7SEG_DISPLAY_4_F
    GPIO_006, 					// PA06         // AVR32_PIN_PA06,                  // 7SEG_DISPLAY_4_G
    GPIO_007, 					// PA07         // AVR32_PIN_PA07,                  // 7SEG_DISPLAY_4_P
    GPIO_008, 					// PA08         // AVR32_PIN_PA08,                  // 7SEG_DISPLAY_3_A
    GPIO_009, 					// PA09         // AVR32_PIN_PA09,                  // 7SEG_DISPLAY_3_B

    GPIO_010, 					// PA10         // AVR32_PIN_PA10,                  // 7SEG_DISPLAY_3_C
    GPIO_011, 					// PA11         // AVR32_PIN_PA11,                  // 7SEG_DISPLAY_3_D
    GPIO_012, 					// PA12         // AVR32_PIN_PA12,                  // 7SEG_DISPLAY_3_E
    GPIO_013, 					// PA13         // AVR32_PIN_PA13,                  // 7SEG_DISPLAY_3_F
    GPIO_014, 					// PA14         // AVR32_PIN_PA14,                  // 7SEG_DISPLAY_3_G
    GPIO_015, 					// PA15         // AVR32_PIN_PA15,                  // 7SEG_DISPLAY_3_P
    GPIO_016, 					// PB00         // AVR32_PIN_PA16,                  // 7SEG_DISPLAY_2_A
    GPIO_017, 					// PB01         // AVR32_PIN_PA17,                  // 7SEG_DISPLAY_2_B
    GPIO_018, 					// PB02         // AVR32_PIN_PA18,                  // 7SEG_DISPLAY_2_C
    GPIO_019, 					// PB03         // AVR32_PIN_PA19,                  // 7SEG_DISPLAY_2_D

    GPIO_020, 					// PB04         // AVR32_PIN_PA20,                  // 7SEG_DISPLAY_2_E
    GPIO_021, 					// PB05         // AVR32_PIN_PA21,                  // 7SEG_DISPLAY_2_F
    GPIO_022, 					// PB06         // AVR32_PIN_PA22,                  // 7SEG_DISPLAY_2_G
    GPIO_023, 					// PB07         // AVR32_PIN_PA23,                  // 7SEG_DISPLAY_2_P
    GPIO_024, 					// PB08         // AVR32_PIN_PA24,                  // 7SEG_DISPLAY_1_A
    GPIO_025, 					// PB09         // AVR32_PIN_PA25,                  // 7SEG_DISPLAY_1_B
    GPIO_026, 					// PB10         // AVR32_PIN_PA26,                  // 7SEG_DISPLAY_1_C
    GPIO_027, 					// PB11         // AVR32_PIN_PA27,                  // 7SEG_DISPLAY_1_D
    GPIO_028, 					// PB12         // AVR32_PIN_PA28,                  // 7SEG_DISPLAY_1_E
    GPIO_029, 					// PB13         // AVR32_PIN_PA29,                  // 7SEG_DISPLAY_1_F

    GPIO_030, 					// PB14         // AVR32_PIN_PA30,                  // 7SEG_DISPLAY_1_G
    GPIO_031,					// PB15         // is not connected to a pad/pin    // 7SEG_DISPLAY_1_P
    GPIO_032, 					// PC00         // AVR32_PIN_PB00,                  // LED_1
    GPIO_033, 					// PC01         // AVR32_PIN_PB01,                  // LED_2
    GPIO_034, 					// PC02         // AVR32_PIN_PB02,                  // LED_3
    GPIO_035, 					// PC03         // AVR32_PIN_PB03,                  // LED_4
    GPIO_036, 					// PC04         // AVR32_PIN_PB04,                  // LED_5
    GPIO_037, 					// PC05         // AVR32_PIN_PB05,                  // LED_6
    GPIO_038, 					// PC06         // AVR32_PIN_PB06,                  // LED_7
    GPIO_039, 					// PC07         // AVR32_PIN_PB07,                  // LED_8

    GPIO_040, 					// PC08         // AVR32_PIN_PB08,                  // SWITCH_1
    GPIO_041, 					// PC09         // AVR32_PIN_PB09,                  // SWITCH_2
    GPIO_042, 					// PC10         // AVR32_PIN_PB10,                  // SWITCH_3
    GPIO_043, 					// PC11         // AVR32_PIN_PB11,                  // SWITCH_4
    GPIO_044, 					// PC12         // AVR32_PIN_PB12,                  // SWITCH_5
    GPIO_045, 					// PC13         // AVR32_PIN_PB13,                  // SWITCH_6
    GPIO_046, 					// PC14         // AVR32_PIN_PB14,                  // SWITCH_7
    GPIO_047, 					// PC15         // AVR32_PIN_PB15,                  // SWITCH_8
    GPIO_048, 					// PD0          // AVR32_PIN_PB16,                  // BUTTON_LEFT
    GPIO_049, 					// PD1          // AVR32_PIN_PB17,                  // BUTTON_DOWN

    GPIO_050, 					// PD2          // AVR32_PIN_PB18,                  // BUTTON_RIGHT
    GPIO_051, 					// PD3          // AVR32_PIN_PB19,                  // BUTTON_UP
    GPIO_052, 					// PD4          // AVR32_PIN_PB20,                  // -not used-
    GPIO_053, 					// PD5          // AVR32_PIN_PB21,                  // -not used-
    GPIO_054, 					// PD6          // AVR32_PIN_PB22,                  // -not used-
    GPIO_055, 					// PD7          // AVR32_PIN_PB23,                  // -not used-
    GPIO_056, 					// PD8          // AVR32_PIN_PB24,                  // -not used-
    GPIO_057, 					// PD9          // AVR32_PIN_PB25,                  // -not used-
    GPIO_058, 					// PD10         // AVR32_PIN_PB26,                  // -not used-
    GPIO_059, 					// PD11         // AVR32_PIN_PB27,                  // -not used-

    GPIO_060, 					// PD12         // AVR32_PIN_PB28,                  // -not used-
    GPIO_061, 					// PD13         // AVR32_PIN_PB29,                  // -not used-
    GPIO_062, 					// PD14         // AVR32_PIN_PB30,                  // -not used-
    GPIO_063, 					// PD15         // AVR32_PIN_PB31,                  // -not used-
    GPIO_064, 					// PE00         // AVR32_PIN_PC00,                  // PMOD_JC_GPIO_1_OUT
    GPIO_065, 					// PE01         // AVR32_PIN_PC01,                  // PMOD_JC_GPIO_2_OUT
    GPIO_066, 					// PE02         // AVR32_PIN_PC02,                  // PMOD_JC_GPIO_3_IN
    GPIO_067, 					// PE03         // AVR32_PIN_PC03,                  // PMOD_JC_GPIO_4_IN
    GPIO_068, 					// PE04         // AVR32_PIN_PC04,                  // -not used-
    GPIO_069, 					// PE05         // AVR32_PIN_PC05,                  // -not used-

    GPIO_070, 					// PE06         // AVR32_PIN_PX34,                  // -not used-
    GPIO_071, 					// PE07         // AVR32_PIN_PX33,                  // -not used-
    GPIO_072, 					// PE08         // AVR32_PIN_PX32,                  // -not used-
    GPIO_073, 					// PE09         // AVR32_PIN_PX31,                  // -not used-
    GPIO_074, 					// PE10         // AVR32_PIN_PX30,                  // -not used-
    GPIO_075, 					// PE11         // AVR32_PIN_PX29,                  // -not used-
    GPIO_076,					// PE12         // AVR32_PIN_PX28,                  // -not used-
    GPIO_077, 					// PE13         // AVR32_PIN_PX27,                  // -not used-
    GPIO_078, 					// PE14         // AVR32_PIN_PX26,                  // -not used-
    GPIO_079, 					// PE15         // AVR32_PIN_PX25,                  // -not used-

    GPIO_080, 					// PF00         // AVR32_PIN_PX24,                  // -not used-
    GPIO_081, 					// PF01         // AVR32_PIN_PX23,                  // -not used-
    GPIO_082, 					// PF02         // AVR32_PIN_PX22,                  // -not used-
    GPIO_083, 					// PF03         // AVR32_PIN_PX21,                  // -not used-
    GPIO_084, 					// PF04         // AVR32_PIN_PX20,                  // -not used-
    GPIO_085, 					// PF05         // AVR32_PIN_PX19,                  // -not used-
    GPIO_086, 					// PF06         // AVR32_PIN_PX18,                  // -not used-
    GPIO_087, 					// PF07         // AVR32_PIN_PX17,                  // -not used-
    GPIO_088, 					// PF08         // AVR32_PIN_PX16,                  // -not used-
    GPIO_089, 					// PF09         // AVR32_PIN_PX15,                  // -not used-

    GPIO_090, 					// PF10         // AVR32_PIN_PX10,                  // -not used-
    GPIO_091, 					// PF11         // AVR32_PIN_PX09,                  // -not used-
    GPIO_092, 					// PF12         // AVR32_PIN_PX08,                  // -not used-
    GPIO_093, 					// PF13         // AVR32_PIN_PX07,                  // -not used-
    GPIO_094, 					// PF14         // AVR32_PIN_PX06,                  // -not used-
    GPIO_095, 					// PF15         // AVR32_PIN_PX05,                  // -not used-
    GPIO_096,                   // PG00         // AVR32_PIN_PX04,
    GPIO_097,                   // PG01         // AVR32_PIN_PX03,
    GPIO_098,                   // PG02         // AVR32_PIN_PX02,
    GPIO_099,                   // PG03         // AVR32_PIN_PX01,

    GPIO_100,                   // PG04         // AVR32_PIN_PX00,
	GPIO_101,                   // PG05         // AVR32_PIN_PX39,
	GPIO_102,                   // PG06         // AVR32_PIN_PX38,
	GPIO_103,                   // PG07         // AVR32_PIN_PX37,
	GPIO_104,                   // PG08         // AVR32_PIN_PX36,
	GPIO_105,                   // PG09         // AVR32_PIN_PX35,
	GPIO_106,                   // PG10         // AVR32_PIN_PX14,
	GPIO_107,                   // PG11         // AVR32_PIN_PX13,
	GPIO_108,                   // PG12         // AVR32_PIN_PX12,
	GPIO_109,                   // PG13         // AVR32_PIN_PX11

    GPIO_110, 					// PG14
    GPIO_111, 					// PG15
    GPIO_112, 					// PH00
    GPIO_113, 					// PH01
    GPIO_114, 					// PH02
    GPIO_115, 					// PH03
    GPIO_116, 					// PH04
    GPIO_117, 					// PH05
    GPIO_118, 					// PH06
    GPIO_119, 					// PH07

    GPIO_120, 					// PH08
    GPIO_121, 					// PH09
    GPIO_122, 					// PH10
    GPIO_123, 					// PH11
    GPIO_124, 					// PH12
    GPIO_125, 					// PH13
    GPIO_126,                   // PH14
    GPIO_127,                   // PH15
    GPIO_128,                   // PI00
    GPIO_129,                   // PI01

    GPIO_130,                   // PI02
	GPIO_131,                   // PI03
	GPIO_132,                   // PI04
	GPIO_133,                   // PI05
	GPIO_134,                   // PI06
	GPIO_135,                   // PI07
	GPIO_136,                   // PI08
	GPIO_137,                   // PI09
	GPIO_138,                   // PI10
	GPIO_139,                   // PI11
	GPIO_INVALID = -1

}GPIO_PIN;


typedef enum {
	GPIO_CFG_OUTPUT_ENABLE = 0,
	GPIO_CFG_NUM_OF_PINS,
	GPIO_CFG_PULLUP_ENABLE,
	GPIO_CFG_PULLDOWN_ENABLE,
	//GPIO_CFG_OPENDRAIN_ENABLE,
	GPIO_CFG_IRQ_SENSITIVITY // falling, rising, both edge(s)
} GPIO_CFG_TYPE;




#ifndef NO_RODOS_NAMESPACE
}
#endif

#endif // _HAL_GPIO_H_
