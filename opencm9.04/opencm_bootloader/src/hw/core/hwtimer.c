/*
 * hwtimer.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */



#include <stdarg.h>
#include <stdbool.h>
#include "hw.h"
#include "hwtimer.h"



typedef struct
{
  TIM_HandleTypeDef hTIM;
  uint8_t  enable;
  uint32_t freq;
  uint32_t prescaler_value;
  uint32_t prescaler_value_1M;
  uint32_t prescaler_div;
  uint32_t period;
  voidFuncPtr handler;
} hwtimer_t;



//-- Internal Variables
//
static hwtimer_t timer_tbl[_HW_DEF_TIMER_CH_MAX];



//-- External Variables
//


//-- Internal Functions
//


//-- External Functions
//




bool hwtimerInit(void)
{
  uint8_t tim_ch;
  uint8_t i;


  //-- TIM1
  //
  tim_ch = _DEF_TIMER1;
  timer_tbl[tim_ch].hTIM.Instance               = TIM1;
  timer_tbl[tim_ch].prescaler_value             = (uint32_t)((SystemCoreClock / 1) / 10000  ) - 1; // 0.01Mhz
  timer_tbl[tim_ch].prescaler_value_1M          = (uint32_t)((SystemCoreClock / 1) / 1000000) - 1; // 1.00Mhz
  timer_tbl[tim_ch].prescaler_div               = 100;
  timer_tbl[tim_ch].hTIM.Init.Period            = 10000 - 1;
  timer_tbl[tim_ch].hTIM.Init.Prescaler         = timer_tbl[tim_ch].prescaler_value;
  timer_tbl[tim_ch].hTIM.Init.ClockDivision     = 0;
  timer_tbl[tim_ch].hTIM.Init.CounterMode       = TIM_COUNTERMODE_UP;
  timer_tbl[tim_ch].hTIM.Init.RepetitionCounter = 0;


  //-- TIM6
  //
  tim_ch = _DEF_TIMER2;
  timer_tbl[tim_ch].hTIM.Instance               = TIM2;
  timer_tbl[tim_ch].prescaler_value             = (uint32_t)((SystemCoreClock / 1) / 10000  ) - 1; // 0.01Mhz
  timer_tbl[tim_ch].prescaler_value_1M          = (uint32_t)((SystemCoreClock / 1) / 1000000) - 1; // 1.00Mhz
  timer_tbl[tim_ch].prescaler_div               = 100;
  timer_tbl[tim_ch].hTIM.Init.Period            = 10000 - 1;
  timer_tbl[tim_ch].hTIM.Init.Prescaler         = timer_tbl[tim_ch].prescaler_value;
  timer_tbl[tim_ch].hTIM.Init.ClockDivision     = 0;
  timer_tbl[tim_ch].hTIM.Init.CounterMode       = TIM_COUNTERMODE_UP;
  timer_tbl[tim_ch].hTIM.Init.RepetitionCounter = 0;



  for( i=0; i<_HW_DEF_TIMER_CH_MAX; i++ )
  {
    timer_tbl[i].handler = NULL;
    timer_tbl[i].enable  = 0;
  }

  return true;
}

void hwtimerStop(uint8_t channel)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  timer_tbl[channel].enable = 0;
  HAL_TIM_Base_DeInit(&timer_tbl[channel].hTIM);
}

void hwtimerSetPeriod(uint8_t channel, uint32_t period_us)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  if( period_us > 0xFFFF )
  {
    timer_tbl[channel].hTIM.Init.Prescaler = timer_tbl[channel].prescaler_value;
    timer_tbl[channel].hTIM.Init.Period    = (period_us/timer_tbl[channel].prescaler_div) - 1;
  }
  else
  {
    if( period_us > 0 )
    {
      timer_tbl[channel].hTIM.Init.Prescaler = timer_tbl[channel].prescaler_value_1M;
      timer_tbl[channel].hTIM.Init.Period    = period_us - 1;
    }
  }
}

void hwtimerAttachInterrupt(uint8_t channel, voidFuncPtr handler)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  hwtimerStop(channel);

  timer_tbl[channel].handler = handler;

}


void hwtimerDetachInterrupt(uint8_t channel)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;


  timer_tbl[channel].handler = NULL;
}

void hwtimerStart(uint8_t channel)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return;

  HAL_TIM_Base_Init(&timer_tbl[channel].hTIM);
  HAL_TIM_Base_Start_IT(&timer_tbl[channel].hTIM);

  timer_tbl[channel].enable = 1;
}

uint32_t hwtimerGetCounter(uint8_t channel)
{
  if( channel >= _HW_DEF_TIMER_CH_MAX ) return 0;

  return timer_tbl[channel].hTIM.Instance->CNT;
}








void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  uint8_t i;


  for (i=0; i<_HW_DEF_TIMER_CH_MAX; i++)
  {
    if (htim->Instance == timer_tbl[i].hTIM.Instance)
    {
      if (timer_tbl[i].handler != NULL)
      {
        (*timer_tbl[i].handler)();
      }
    }
  }
}


void TIM1_UP_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer_tbl[_DEF_TIMER1].hTIM);
}
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&timer_tbl[_DEF_TIMER2].hTIM);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  if( htim->Instance == TIM1 )
  {
    __HAL_RCC_TIM1_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM1_UP_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
  }
  if( htim->Instance == TIM2 )
  {
    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM2_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  if( htim->Instance == TIM1 )
  {
    HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
  }
  if( htim->Instance == TIM2 )
  {
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  }
}


