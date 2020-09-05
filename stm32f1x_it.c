 
#include "Header.h"
uint16_t IC2Value = 0,IC1Value=0;
uint16_t pulseWidth = 0;
uint32_t Frequency = 0;

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{}

/*int d2i(float d)
{
 float value; 
 value = d; 
 value /= 10; 
 value += 0.5; 
 return value; 
}*/

  /* Enable the CC2 Interrupt Request */
 




void TIM2_IRQHandler(void)
{
  /* Clear TIM3 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

  /* Get the Input Capture value */
  IC2Value = TIM_GetCapture2(TIM2);

  if (IC2Value != 0)
  {
    IC1Value=TIM_GetCapture1(TIM2);
    /* Duty cycle computation */
    pulseWidth = (IC1Value*2);
    
    /* Frequency computation */
    Frequency =  (50000 /  IC2Value);  //SystemCoreClock /IC2Value ;
  }
  else
  {
    pulseWidth = 0;
    Frequency = 0;
  }
   TIM1->SR = ~0xFF;
}


 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/