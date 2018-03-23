/*
 * button.c
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */


#include "button.h"





void buttonInit(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin = GPIO_PIN_12;
  gpio_init_structure.Mode = GPIO_MODE_INPUT;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_LOW;

  HAL_GPIO_Init(GPIOC, &gpio_init_structure);


  gpio_init_structure.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

}

bool buttonGetPressed(uint8_t ch)
{
  bool ret = false;


  switch(ch)
  {
    case 0:
      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) == GPIO_PIN_SET)
      {
        ret = true;
      }
      break;

    case 1:
      if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3) == GPIO_PIN_SET)
      {
        ret = true;
      }
      break;
  }

  return ret;
}
