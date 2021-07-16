/*
 * @file hw_timer.cpp
 * @date 2012/08/22
 * @author Michael Ruffer
 *
 * Copyright 2012 University of Wuerzburg
 *
 * @brief Timer for system time and preemption
 */
#include "rodos.h"
#include "hw_specific.h"

#include "default-platform-parameter.h"

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dbgmcu.h"
#include "core_cm4.h"



#ifndef NO_RODOS_NAMESPACE
namespace RODOS {
#endif


/***** Preemption Timer - Cortex "SysTick" counter *****
 *
 * Timer to force scheduler calls -> preemption
 * - cortex system counter is used -> "SysTick"
 * - 24Bit counter running at CPU_CLK/8 (168MHz/8 = 21MHz)
 * -> max. value: 0xFFF = 2^24 = 16.777.216
 * -> 1ms @ 21MHz: 21.000
 * -> 10ms  @ 21MHz: 210.000
 * -> 100ms  @ 21MHz: 2.100.000
 * -> !!! 1s  @ 21MHz: 21.000.000 -> counter overflow !!!
 */
extern long long timeToTryAgainToSchedule;
extern bool isSchedulingEnabled;


extern "C" {
/*
 * Interrupt Service Routine for "SysTick" counter
 *
 * !!! Don't use "naked" for the ISR, because the system crashes
 * -> maybe because not all registers are saved automatically
 * -> it works when the compiler puts prologue and epilogue in the ISR
 *    -> but this can be a problem when ISRs can interrupt each other
 * -> this can happen when they don't have the same priority !!!
 */
void SysTick_Handler();
void SysTick_Handler() {

	if(!isSchedulingEnabled) return;

	long long timeNow = NOW();  // comment this out to improve performance, but: no time events any more
	TimeEvent::propagate(timeNow); // comment this out to improve performance, but: no time events any more

	if(NOW() < timeToTryAgainToSchedule) return;

	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // set SW-IRQ to call scheduler
}


/** \brief  System Tick Configuration

    This function initialises the system tick timer and its interrupt WITHOUT starting it
    Counter is in free running mode to generate periodical interrupts.

    \param [in]  ticks  Number of ticks between two interrupts
    \return          0  Function succeeded
    \return          1  Function failed
 */
static __INLINE uint32_t SysTick_Config_New(uint32_t ticks)
{
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */

  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
//                   SysTick_CTRL_TICKINT_Msk   |
//                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}

static __INLINE uint32_t SysTick_IRQEnable(void)
{
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	return (0);
}


static __INLINE uint32_t SysTick_IRQDisable(void)
{
	SysTick->CTRL &= ~( SysTick_CTRL_TICKINT_Msk);

	return (0);
}

static __INLINE uint32_t SysTick_Enable(void)
{
	SysTick->CTRL = 	// SysTick_CTRL_CLKSOURCE_Msk	| // if set clock source = CPU_CLK else CPU_CLK/8
						SysTick_CTRL_TICKINT_Msk	|
						SysTick_CTRL_ENABLE_Msk;

	return (0);
}


static __INLINE uint32_t SysTick_Disable(void)
{
	SysTick->CTRL = 0;

	return (0);
}


} // end extern "C"

/** 
* the timer interval
*/
long long Timer::microsecondsInterval = PARAM_TIMER_INTERVAL;

void Timer::init() 
{
	SysTick_Config_New(static_cast<uint32_t>((SystemCoreClock/8) * Timer::microsecondsInterval / 1000000)); // initialization of systick timer
}

/**
* start timer 
*/
void Timer::start() 
{
	SysTick_Config_New(static_cast<uint32_t>((SystemCoreClock/8) * Timer::microsecondsInterval / 1000000)); // initialization of systick timer
	SysTick_Enable();
}

/**
* stop timer 
*/
void Timer::stop() 
{
	SysTick_Disable();
}

/**
* set timer interval 
*/
void Timer::setInterval(const int64_t microsecondsInterval) {
  Timer::microsecondsInterval = microsecondsInterval;
}




/***** System Time ******
 *
 * timer is used to generate the system time
 * -> interrupt every 10ms
 * -> time resolution: 1/timerClock = 166,6ns
 * -> max. timerCount = timerClock/100 = 60 000 (IRQ every 10ms)
 */
static const unsigned int timerClock = 6000000;

/* timer 2,3,4,5,6,7,12,13,14 running at APB1_CLK*2 = 84MHz
 * timer 1,8,10,11 running at APB2_CLK*2 = 168MHz
 * timer 2,5: 32 Bit
 * timer 1,3,4,6,7,8,9,10,11,12,13,14: 16 Bit
 */
/* Timer 2: 32Bit, APB1 */
//#define TIMx                    TIM2
//#define RCC_APBnPeriph_TIMx     RCC_APB1Periph_TIM2
//#define DBGMCU_TIMx_STOP        DBGMCU_TIM2_STOP
//#define TIMx_IRQn               TIM2_IRQn
//#define TIMx_IRQHandler         TIM2_IRQHandler
//#define RCC_APBnPeriphClockCmd  RCC_APB1PeriphClockCmd
//#define RCC_APBnPeriphResetCmd  RCC_APB1PeriphResetCmd
//#define PCLKn_Frequency         PCLK1_Frequency

/* Timer 7: 16Bit, APB1 */
//#define TIMx                    TIM7
//#define RCC_APBnPeriph_TIMx     RCC_APB1Periph_TIM7
//#define DBGMCU_TIMx_STOP        DBGMCU_TIM7_STOP
//#define TIMx_IRQn               TIM7_IRQn
//#define TIMx_IRQHandler         TIM7_IRQHandler
//#define RCC_APBnPeriphClockCmd  RCC_APB1PeriphClockCmd
//#define RCC_APBnPeriphResetCmd  RCC_APB1PeriphResetCmd
//#define PCLKn_Frequency         PCLK1_Frequency

/* Timer 11: 16Bit, APB2 */

#define TIMx                    TIM11
#define RCC_APBnPeriph_TIMx     RCC_APB2Periph_TIM11
#define DBGMCU_TIMx_STOP        DBGMCU_TIM11_STOP
#define TIMx_IRQn               TIM1_TRG_COM_TIM11_IRQn
#define TIMx_IRQHandler         TIM1_TRG_COM_TIM11_IRQHandler
#define RCC_APBnPeriphClockCmd  RCC_APB2PeriphClockCmd
#define RCC_APBnPeriphResetCmd  RCC_APB2PeriphResetCmd
#define PCLKn_Frequency         PCLK2_Frequency

/* Timer 14: 16Bit, APB1 */
//#define TIMx                    TIM14
//#define RCC_APBnPeriph_TIMx     RCC_APB1Periph_TIM14
//#define DBGMCU_TIMx_STOP        DBGMCU_TIM14_STOP
//#define TIMx_IRQn               TIM8_TRG_COM_TIM14_IRQn
//#define TIMx_IRQHandler         TIM8_TRG_COM_TIM14_IRQHandler
//#define RCC_APBnPeriphClockCmd  RCC_APB1PeriphClockCmd
//#define RCC_APBnPeriphResetCmd  RCC_APB1PeriphResetCmd
//#define PCLKn_Frequency         PCLK1_Frequency


int64_t nanoTime;

extern "C" {
/*
 * Interrupt Service Routine for Timer
 *
 * !!! Don't use "naked" for the ISR, because the system crashes
 * -> maybe because not all registers are saved automatically
 * -> it works when the compiler puts prologue and epilogue in the ISR
 *    -> but this can be a problem when ISRs can interrupt each other
 * -> this can happen when they don't have the same priority !!!
 */
void TIMx_IRQHandler();
void TIMx_IRQHandler()
{
   TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
   NVIC_ClearPendingIRQ(TIMx_IRQn);
   nanoTime += 10000000; // 10M ns for each 10ms-tick
}
} // end extern "C"

void TIMx_init();
void TIMx_init(){
    // local variables to initialize timer
    TIM_TimeBaseInitTypeDef timStruct;
    unsigned char timClockMultiplier = 0;
    RCC_ClocksTypeDef clocks;

    // init timer for system time
    RCC_APBnPeriphClockCmd(RCC_APBnPeriph_TIMx, ENABLE);
    RCC_APBnPeriphResetCmd(RCC_APBnPeriph_TIMx, DISABLE);
    RCC_GetClocksFreq(&clocks);

    TIM_DeInit(TIMx);

    // if the APBn Prescaler is bigger than 1, the PCLKn is multiplied by 2 infront of the timer
    if(clocks.HCLK_Frequency/clocks.PCLKn_Frequency > 1)
    { timClockMultiplier = 2; }
    else
    { timClockMultiplier = 1; }

    // TIM_Period = timer-clk divided by 100 to get an interrupt every 10ms
    timStruct.TIM_Period = timerClock / 100;

    // Prescaler
    // fCK_CNT = fCK_PSC/(TIM_Prescaler+1) -> TIM_Prescaler = (fCK_PSC/fCK_CNT)-1
    timStruct.TIM_Prescaler         = static_cast<uint16_t>(((clocks.PCLKn_Frequency*timClockMultiplier) / timerClock) -1);

    // timer mode is "count up"
    timStruct.TIM_CounterMode       = TIM_CounterMode_Up;

    // TIM_ClockDivision = division ratio between the timer clock (CK_INT)
    // frequency and sampling clock used by the digital filters
    // -> not interesting for our purpose !!!
    timStruct.TIM_ClockDivision     = TIM_CKD_DIV1;

    // only valid for TIM1 and TIM8
    timStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIMx,&timStruct);
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
    DBGMCU_Config(DBGMCU_TIMx_STOP, ENABLE);            // stop Timer during debug break
    TIM_Cmd(TIMx, ENABLE);

    NVIC_SetPriority(TIMx_IRQn, 255);
    // enable timer-interrupt in interrupt controller
    NVIC_EnableIRQ(TIMx_IRQn);
}

int64_t hwGetNanoseconds() {

	uint32_t count = 0;
	int64_t returnTime = 0;

	// -> current time = nanoTime + 1 000 000 000 * countRegister/tim2Clock

	// Disable Interrupt is no solution here (wrong values caused by missed interrupt)

	// Read nanoTime twice, to make sure it has not changed while reading counter value
	do {
		returnTime = nanoTime;
		count = TIM_GetCounter(TIMx);
	} while(returnTime != nanoTime);


	/** low precision
	 * - nanos = 166ns * count (for tim2Clock = 6MHz)
	 * - nanos = 11ns * count (for tim2Clock = 84MHz)
	 * ! startup time might be wrong due to integer overflow !
	 */
	//int nanos = 1000000000/timerClock * count;

	/** high precision
	 * - nanos = 166,666666ns * count (for tim2Clock = 6MHz)
	 * - nanos = 11,904761s * count (for tim2Clock = 84MHz)
	 * - takes 4 times longer than low precision
	 */
	long long nanos = 0;
	nanos = 1000000000/(timerClock/1000000);
	nanos *= count;
	nanos /= 1000000;

	return returnTime + nanos;
}


void hwInitTime()
{
	nanoTime=0;
}


int64_t hwGetAbsoluteNanoseconds(void)
{
	return hwGetNanoseconds();// + timeAtStartup;
}



#ifndef NO_RODOS_NAMESPACE
}
#endif

