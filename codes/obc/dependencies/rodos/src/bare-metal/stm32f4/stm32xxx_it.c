/**
  ******************************************************************************
  * @file    stm32xxx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Exceptions Handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/
#ifdef STM32F429_439xx
/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_ISR_Handler(void)__attribute__((weak));
void LTDC_ISR_Handler(void){
	while(1){}
}

void LTDC_IRQHandler(void)
{
  LTDC_ISR_Handler();
}

/**
  * @brief  This function handles DMA2D global interrupt request.
  * @param  None
  * @retval None
  */
void DMA2D_ISR_Handler(void)__attribute__((weak));
void DMA2D_ISR_Handler(){
	while (1){}
}

void DMA2D_IRQHandler(void)
{
  DMA2D_ISR_Handler();
}
#endif

void NMI_Handler();
void NMI_Handler(){
	while(1){}
}

void HardFault_Handler();
void HardFault_Handler(){
	//Put PC that caused fault in R1
	  __asm volatile (
	    " movs r0,#4       \n"
	    " movs r1, lr      \n"
	    " tst r0, r1       \n"
	    " beq _MSP         \n"
	    " mrs r0, psp      \n"
	    " b _HALT          \n"
	  "_MSP:               \n"
	    " mrs r0, msp      \n"
	  "_HALT:              \n"
	    " ldr r1,[r0,#20]  \n"
	    " bkpt #0          \n"
	  );
	while(1){}
}

void MemManage_Handler();
void MemManage_Handler(){
	while(1){}
}

void BusFault_Handler();
void BusFault_Handler(){
	while(1){}
}

void UsageFault_Handler();
void UsageFault_Handler(){
	while(1){}
}

void WWDG_IRQHandlerFunc();
void WWDG_IRQHandlerFunc(){
	while(1){}
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
