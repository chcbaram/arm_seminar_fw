/*
 * timer.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */


#include "timer.h"



static TIM_HandleTypeDef    TimHandle;
static uint32_t uwPrescalerValue = 0;



static void (*timer_isr_func)(void) = NULL;



void timerInit(void)
{

}

void timerSetISR(void (*func)(void), uint32_t time_us)
{
  timer_isr_func = func;

  __TIM3_CLK_ENABLE();

  /* Compute the prescaler value to have TIMx counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 1000000) - 1;

  /* Set TIMx instance */
  TimHandle.Instance = TIM3;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = ((SystemCoreClock / 2)/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period            = time_us - 1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&TimHandle);

  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void timerStart(void)
{
  HAL_TIM_Base_Start_IT(&TimHandle);
}


void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)
  {
    if (timer_isr_func != NULL)
    {
      (*timer_isr_func)();
    }
  }
}

