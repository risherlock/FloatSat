This port uses:
UART            for stdout via xprintf/PRINTF -> can be changed in params.h
Timer 11        for RODOS time base
SysTick Timer   for preemption
40kB RAM        for Thread stacks -> can be changed in params.h

During startup is used:
Timer 5         for Watchdog init -> can be used after hwInit() (see hw_specific.cpp for details on redefining ISR)

TODO: free Timer 11 -> use also SysTick timer for RODOS time base

Supported devices:
* STM32F407 (DISCOVERY)
	compiler flags -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER
* STM32F429 (DISCO (touch screen))
	compiler flags -DSTM32F429_439xx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER
* STM32F427 
	compiler flags -DSTM32F427_437xx -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER
* STM32F415 (MINI-M4)
	compiler flags -DSTM32F40_41xxx -DHSE_VALUE=16000000 -DUSE_STDPERIPH_DRIVER
* STM32F401 (NUCLEO)
	compiler flags -DSTM32F401xx -DUSE_STDPERIPH_DRIVER
	OPENOCD version 0.8.0 or higher needed
	since no external clock available, HSI used
	when using UART2, close solder bridge (sb) 62, 63 and open sb 13, 14
	
