
/**
 * @file hw_specific.cc
 * @date 2008/04/23 7:33
 * @port 2010/02/18 19:14
 * @author Miroslaw Sulejczak, modified by Michael Ruffer
 *
 *
 * @brief all hardware specific stuff I have no better place for ...
 *
 */

#include <sys/stat.h>
#include <errno.h>

#include "rodos.h"

#include "default-platform-parameter.h"
#include "hw_specific.h"
#include "hal/hal_uart.h"

#include "stm32f4xx.h"
#include "core_cm4.h"

// watchdog related includes
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_misc.h"
#include "stm32f4xx_dbgmcu.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_exti.h"

volatile long *contextT;

namespace RODOS {

// - on interrupt the cortex m3 saves automatically the context of the interrupted function
// - it saves the registers xPSR,PC,LR,r12,r3,r2,r1,r0 on stack
// - that is why it's not necessary that the compiler adds code to save the context, so
//   we have to add the attribute "naked" to avoid this
extern "C" {
void PendSV_Handler(void) __attribute__ (( naked ));
void SVC_Handler(void) __attribute__ (( naked ));
}

/** function to do hardware initialization 
 * -> hwInit() is called in main() (src/independent/main.cpp)
 */

HAL_UART uart_stdout(UART_DEBUG);

void TIMx_init(); // timer for system time -> see hw_timer.cpp

/* measure LSI frequency using Timer 5 -> necessary for watchdog
 * -> only once in hwInit() -> afterwards Timer 5 is free
 */
static uint32_t GetLSIFrequency(void);
static uint32_t LSIFrequency = 0;
static void initRtcInterrupt();

void hwInit() {
	schedulerRunning = false;

	/* Make PendSV, SysTick and Timer2 the same priroity */
	NVIC_SetPriority(PendSV_IRQn, 255);
	NVIC_SetPriority(SysTick_IRQn, 255);
	//NVIC_SetPriority(TIM2_IRQn, 255); // moved to TIMx_init()

	// initialization of stm32f4:
	// set system clock to 168MHz
	// init RCC
	// ...
//	SystemInit(); // is called in startup code before constructor call and main
	SystemCoreClockUpdate(); // update a global variable holding the system clock

	LSIFrequency = GetLSIFrequency();

	//Determine the Node Number from the first bytes of the chips UID
	extern int32_t myNodeNr;
	#define STM32F4_UID ((unsigned char*)(0x1FFF7A10))
	#define STM32F4_UID_SIZE 12
	myNodeNr= *(long*)STM32F4_UID;

	//UART for stdout
	uart_stdout.init(115200);
	//uart_stdout.config(UART_PARAMETER_ENABLE_DMA,32);
	//uart_stdout.config(UART_PARAMETER_HW_FLOW_CONTROL, 1); // necessary for bluetooth module

	TIMx_init(); // Timer for system time

	RCC_ClocksTypeDef  rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	PRINTF("CPU: %ld MHz\n",rcc_clocks.SYSCLK_Frequency/1000000);

	// Enable debugging / programming in sleep mode
	DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, ENABLE);

	initRtcInterrupt();
}

void initRtcInterrupt() {
	EXTI_InitTypeDef  EXTI_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);

	/* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

	/* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();

  /* RTC Wakeup Interrupt Generation: Clock Source: RTCCLK_Div16

     Wakeup Time Base = (16 / (LSE or LSI)) * WakeUpCounter
  */
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);

  /* Enable the Wakeup Interrupt */
  RTC_ITConfig(RTC_IT_WUT, ENABLE);

  /* Connect EXTI_Line22 to the RTC Wakeup event */
  EXTI_ClearITPendingBit(EXTI_Line22);
  EXTI_InitStructure.EXTI_Line = EXTI_Line22;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable the RTC Wakeup Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 *  abort execution
 */
void hwResetAndReboot() {
    //NVIC_SystemReset();
    isShuttingDown = true;
    hwInitWatchdog(0);
    while(1){}
}

/**
  *  global interrupt disable
  */
void hwDisableInterrupts(){
    __disable_irq();
}

/**
  *  global interrupt enable
  */
void hwEnableInterrupts(){
    __enable_irq();
}
/**
 * In case a network is avialable, the corresponding node nr.
 */

// TW: already defind in rodos/src/independent/globaldata.cpp
//int32_t myNodeNr = -1 ;
//long getNodeNumber() { return myNodeNr; }

extern "C" {
/*
 * SVC_Handler is only used to start the first thread
 * - We do this procedure to load rest of context of first thread (idleThread) and switch to PSP
 * -> PSP (Process Stack Pointer) is used in threads
 * -> MSP (Main Stack Pointer) is used in exception handlers (so in scheduler)
 * - MSP is the default stack pointer after reset
 */
void SVC_Handler(void) {
	__asm volatile (
			"	ldr	r1, =contextT				\n" // after start "contextT" holds the stack pointer of first thread
			"	ldr r0, [r1]					\n"// load stack pointer of first thread (idleThread)
			"	ldmia r0!, {r4-r11, r14}		\n"// Pop the registers that are not automatically saved on exception entry and the critical nesting count.
			"	msr psp, r0						\n"// load stack pointer of first thread to PSP -> PSP is used after leaving ISR
			"	bx r14							\n"
			"									\n"
			"	.align 2						\n"
	);
}

/* When the ISR is entered the following registered are saved automatically:
 * FPSCR, S15 ... S0, xPSR, PC, LR, R12, R3, R2, R1, R0
 * -> the other registers (R11 ... R4, S31 ... S16) must be saved and restored manually
 * -> very helpful document: "PM0214 Programming manual - STM32F3xxx and STM32F4xxx Cortex-M4 programming manual" page 42
 */
void PendSV_Handler(void) {
	__asm volatile
	(
			"	cpsid f								\n" // disable all interrupts
			"	mrs r0, psp							\n"// load stack pointer of interrupted thread in r0 -> r0 is used to save all not automatically saved core and fpu registers
			"										\n"
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
			"	tst r14, #0x10						\n" // Is the task using the FPU context?  If so, push high vfp registers.
			"	it eq								\n"
			"	vstmdbeq r0!, {s16-s31}				\n"// save FPU registers on stack of interrupted thread
#endif
			"										\n"
			"	stmdb r0!, {r4-r11, r14}			\n" // save core registers on stack of interrupted thread
			"										\n"
			"	bl schedulerWrapper					\n"
			"										\n"
			"	ldr r1, =contextT					\n"// "contextT" holds the stack pointer of next thread
			"	ldr r0, [r1]						\n"// load stack pointer of next thread
			"										\n"
			"	ldmia r0!, {r4-r11, r14}			\n"// Pop the core registers
			"										\n"
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
			"	tst r14, #0x10						\n" // Is the task using the FPU context?  If so, pop the high vfp registers too
			"	it eq								\n"
			"	vldmiaeq r0!, {s16-s31}				\n"
#endif
			"										\n"
			"	msr psp, r0							\n" // load psp with stack pointer of next thread -> PSP (process stack pointer) is used after leaving ISR
			"	cpsie f								\n"// enable all interrupts
			"	bx r14								\n"
			"										\n"
			"	.align 2							\n"
	);
}
}

/*
 * Initialize and start HW-watchdog
 * - it is used the IWDG (Independent Watchdog) of the STM32F4
 * - it is clocked by it's own independent low-speed clock (LSI)
 * - LSI can vary between 17 kHz and 47 kHz (32kHz typical)
 *   -> so it must be measured using Timer 5 -> done in hwInit
 * - max. interval@47kHz: 1/(47kHz/256) * 0xFFF = 22305 ms
 * - max. interval@32kHz: 1/(32kHz/256) * 0xFFF = 32768 ms
 * - max. interval@17kHz: 1/(17kHz/256) * 0xFFF = 61666 ms
 */
void hwInitWatchdog(int32_t intervalMilliseconds) {
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable );

	/* set IWDG counter clock prescaler depending on intervalMilliseconds to improve range and accuracy*/
	uint32_t prescaler = 0;
	if (intervalMilliseconds < 1000) {
		IWDG_SetPrescaler(IWDG_Prescaler_16 );
		prescaler = 16;
	} else {
		IWDG_SetPrescaler(IWDG_Prescaler_256 );
		prescaler = 256;
	}

	/* Set counter reload value
	 Counter Reload Value = intervalMilliseconds / IWDG counter clock period
	 = intervalMilliseconds / (LsiFreq/prescaler)
	 */
	double wdgClk = (double) LSIFrequency / (double) prescaler;
	double intervalSec = (double) intervalMilliseconds / (double) 1000;
	uint32_t reloadVal = static_cast<uint32_t>(intervalSec * wdgClk);
	if (reloadVal > 0xFFF) {
		reloadVal = 0xFFF; // set to maximum reload value
	}
	IWDG_SetReload(static_cast<uint16_t>(reloadVal));

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

/*
 * trigger HW-watchdog -> must be done periodically with less than "intervalMilliseconds" ms
 */
void hwTriggerWatchdog() {
	IWDG_ReloadCounter();
}

/**
 * @brief  This function handles TIM5 global interrupt during LSI measurement on startup.
 *         This is done only once in hwInit().
 * @param  None
 * @retval None
 */
static volatile unsigned int PeriodValue = 0;
static volatile bool captureRdy = false;
static uint32_t CaptureNumber = 0;

extern "C" {


void TIM5_ISR4LSI();
void TIM5_ISR4LSI(){
    static uint16_t tmpCC4[2] = { 0 };

    if (TIM_GetITStatus(TIM5, TIM_IT_CC4 ) != RESET) {
        /* Get the Input Capture value */
    	if(CaptureNumber < 2){
    		tmpCC4[CaptureNumber++] = (uint16_t)TIM_GetCapture4(TIM5 );
    	}

        /* Clear CC4 Interrupt pending bit */
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC4 );

        if (CaptureNumber >= 2) {
            /* Compute the period length */
            PeriodValue = (uint16_t) (0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);
            CaptureNumber = 0;
            captureRdy = true;
        }
    }
}


/* when redefining TIM5_ISR_USER remember to put it in namespace RODOS
 * Example:
    #ifndef NO_RODOS_NAMESPACE
    namespace RODOS {
    #endif
        void TIM5_ISR_USER(){
            // do your stuff
        }
    #ifndef NO_RODOS_NAMESPACE
    }
    #endif
 */
void TIM5_ISR_USER()__attribute__((weak));
void TIM5_ISR_USER(){}

/**
 * @brief  This function handles TIM5 global interrupt request.
 * @param  None
 * @retval None
 */

void TIM5_IRQHandler(void);
void TIM5_IRQHandler(void) {
    if (LSIFrequency == 0){
        TIM5_ISR4LSI();
    }else{
        TIM5_ISR_USER();
    }
}
} // end extern "C"

/**
 * @brief  Configures TIM5 to measure the LSI oscillator frequency.
 * 		The LSI frequency is used to init the Independent Watchdog.
 * @param  None
 * @retval LSI Frequency
 */
static uint32_t GetLSIFrequency(void) {
    if (LSIFrequency != 0) return LSIFrequency;

	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	RCC_ClocksTypeDef RCC_ClockFreq;

	// initialize all global variables -> that's really important if function is called more than one time
	PeriodValue = 0;
	captureRdy = false;
	CaptureNumber = 0;

	/* Enable the LSI oscillator ************************************************/
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY ) == RESET) {
	}

	/* TIM5 configuration *******************************************************/
	/* Enable TIM5 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
	TIM_RemapConfig(TIM5, TIM5_LSI );

	/* Configure TIM5 presclaer */
	TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate );

	/* TIM5 configuration: Input Capture mode ---------------------
	 The LSI oscillator is connected to TIM5 CH4
	 The Rising edge is used as active edge,
	 The TIM5 CCR4 is used to compute the frequency value
	 ------------------------------------------------------------ */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	/* Enable TIM5 Interrupt channel */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable TIM5 counter */
	TIM_Cmd(TIM5, ENABLE);

	/* Reset the flags */
	TIM5->SR = 0;

	/* Enable the CC4 Interrupt Request */
	TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);

	/* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() ) */
	while (!captureRdy) {
	}

	/* Deinitialize the TIM5 peripheral registers to their default reset values */
	TIM_DeInit(TIM5 );

	/* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
	/* Get SYSCLK, HCLK and PCLKx frequency */
	RCC_GetClocksFreq(&RCC_ClockFreq);

	/* Get PCLK1 prescaler */
	if ((RCC ->CFGR & RCC_CFGR_PPRE1 )== 0){
		/* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
		return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);
	} else { /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
		return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8);
	}
}

void enterSleepMode() {
	__WFI();
}

extern int64_t nanoTime;
long long deepSleepWakeupTime = 0;

void deepSleepUntil(long long until) {
	if(until < NOW())
		return;

  SysTick->CTRL  &= ~SysTick_CTRL_TICKINT_Msk;        // systick IRQ off

	uint32_t cr = RCC->CR;
	uint32_t cfgr = RCC->CFGR;
	uint32_t pllcfgr = RCC->PLLCFGR;

  RCC->BDCR |= RCC_BDCR_LSEON;
  RCC->BDCR |= RCC_BDCR_RTCEN;
  EXTI->PR = (1 << 22);

  PWR_ClearFlag(PWR_FLAG_WU);

	long long interval = static_cast<long long>((double)(until - NOW()) / 1000000.0 / 0.5);
	RTC_SetWakeUpCounter(static_cast<uint32_t>(interval));
	RTC_WakeUpCmd(ENABLE);

	deepSleepWakeupTime = until;

	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

	RTC_WakeUpCmd(DISABLE);

	RCC->CR = cr;
	RCC->CFGR = cfgr;
	RCC->PLLCFGR = pllcfgr;

  SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;            // systick IRQ on
}

extern "C"
{

void RTC_WKUP_IRQHandler()
{
  if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
  {
		if(deepSleepWakeupTime > 0)
    	nanoTime = deepSleepWakeupTime;

		deepSleepWakeupTime = 0;

    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line22);
  }
}

}

#ifndef NO_RODOS_NAMESPACE
}
#endif

