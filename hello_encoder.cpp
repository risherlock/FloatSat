// Encoder interface using STM32F4's encoder mode on PA0 and PA1.
// Note: Changing pin is not trivial and not every pins support encoder mode.

#include "rodos.h"
#include "stm32f4xx.h"

class HelloEncoder : public StaticThread<>
{
private:
  uint16_t period = 100 ; // ms
  void init_encoder(void);
  int get_count(void);

public:
  void init();
  void run();
} hello_encoder;

void HelloEncoder::init()
{
  init_encoder();
}

void HelloEncoder::run()
{
  int last_cnt = 0;

  TIME_LOOP(NOW(), period * MILLISECONDS)
  {
    int cnt = get_count();

    if(cnt != last_cnt)
    {
      PRINTF("Counts: %d\r\n", get_count());
      last_cnt = cnt;
    }
  }
}

void HelloEncoder::init_encoder(void)
{
  // Encoder pins config: PA0 and PA1
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER |= GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1;
  GPIOA->AFR[0] |= (1 << 0 * 4) | (1 << 1 * 4);

  // TIM2 encoder mode 3. RM0090 Rev 18, Pg. 552
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
  TIM2->CCER &= ~(TIM_CCER_CC1NP | TIM_CCER_CC1P);
  TIM2->CCER &= ~(TIM_CCER_CC2NP | TIM_CCER_CC2P);
  TIM2->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
  TIM2->CCMR1 &= ~(TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_3);
  TIM2->CCMR1 &= ~(TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_3);
  TIM2->CR1 |= TIM_CR1_CEN;
}

int HelloEncoder::get_count(void)
{
  return TIM2->CNT;
}
