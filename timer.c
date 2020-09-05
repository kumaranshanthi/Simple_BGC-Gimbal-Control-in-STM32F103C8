#include "Header.h"


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;


void timerInit(TIM_TypeDef* TIMx,unsigned int clock,unsigned int pwmfreq)
{
 
  /* Compute the prescaler value */
  uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / clock) - 1; // 1Mhz Timer Clock 
  
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = clock/pwmfreq; // 50Hz  = 1Mhz / 20000
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
}
int timerChannelPwmOutConfig(TIM_TypeDef* TIMx,uint16_t channel,int pwmPulse)
{
   /* PWM1 Mode configuration */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = pwmPulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  switch(channel)
  {
    case 1:
          TIM_OC1Init(TIMx, &TIM_OCInitStructure);
          TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
          break;
    case 2:
          TIM_OC2Init(TIMx, &TIM_OCInitStructure);
          TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
          break;
    case 3:
          TIM_OC3Init(TIMx, &TIM_OCInitStructure);
          TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
          break;
    case 4:
          TIM_OC4Init(TIMx, &TIM_OCInitStructure);
          TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
          break;
    default:
          return -1;
  }  
  return 0;
}
void timer(TIM_TypeDef* TIMx, FunctionalState NewState)
{   
  TIM_ARRPreloadConfig(TIMx, NewState);
  TIM_Cmd(TIMx, NewState);
}

int updateTimerPwmDutyCycle(TIM_TypeDef* TIMx,uint16_t channel,uint16_t dutycycle)
{
  switch(channel)
  {
    case 1:
          TIMx->CCR1 = dutycycle;
          break;
    case 2:
          TIMx->CCR2 = dutycycle;
          break;
    case 3:
          TIMx->CCR3 = dutycycle;
          break;
    case 4:
          TIMx->CCR4 = dutycycle;
          break;
    default:
          return -1;
  }  
  return 0;
}