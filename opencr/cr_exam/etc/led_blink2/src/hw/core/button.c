/*
 * button.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */


#include "button.h"



void buttonInit(void)
{
  GPIO_InitTypeDef  gpio_init_structure;


  gpio_init_structure.Pin   = GPIO_PIN_12;
  gpio_init_structure.Mode  = GPIO_MODE_INPUT;
  gpio_init_structure.Pull  = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_LOW;

  HAL_GPIO_Init(GPIOC, &gpio_init_structure);
}

bool buttonGetPressed(uint8_t ch)
{
  bool ret = false;


  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_SET)
  {
    ret = true;
  }

  return ret;
}
