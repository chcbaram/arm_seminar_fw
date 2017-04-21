/*
 *  buzzer.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */
#include "hw.h"
#include "buzzer.h"




volatile int32_t  buzzer_toggle_count;
volatile uint8_t  buzzer_timer = _HW_DEF_BUZZER_CH;
volatile bool     buzzer_enable = false;


static void buzzerBegin(void);
static void buzzerEnd(void);



void buzzer_isr(void)
{
  if (buzzer_toggle_count != 0)
  {
    HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_14);
    if(buzzer_toggle_count > 0)
    {
      buzzer_toggle_count--;
    }
  }
  else
  {
    buzzerEnd();
  }
}



bool buzzerInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;


  GPIO_InitStruct.Pin  = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);


  return true;
}

void buzzerBegin(void)
{
  buzzer_enable  = true;
}


void buzzerEnd(void)
{
  buzzer_enable = false;
  hwtimerStop(buzzer_timer);
}

void buzzerStart
(uint32_t frequency, uint32_t duration)
{
  uint32_t toggle_count = 0;
  uint32_t ocr = 0;



  ocr = (1000000 / frequency) / 2 ;

  // Calculate the toggle count
  if (duration > 0)
  {
    toggle_count = 2 * frequency * duration / 1000;
  }
  else
  {
    toggle_count = -1;
  }

  buzzer_toggle_count = toggle_count;

  buzzerBegin();

  hwtimerSetPeriod(_HW_DEF_BUZZER_CH, ocr);
  hwtimerAttachInterrupt(_HW_DEF_BUZZER_CH, buzzer_isr);
  hwtimerStart(_HW_DEF_BUZZER_CH);
}

