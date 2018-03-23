/*
 * led.c
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */


#include "led.h"




void ledInit(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Configure the GPIO_LED pin */
  gpio_init_structure.Pin = GPIO_PIN_12;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_HIGH;

  HAL_GPIO_Init(GPIOG, &gpio_init_structure);


  gpio_init_structure.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);


  /* By default, turn off LED */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);
}

void ledOn(uint8_t ch)
{
  switch(ch)
  {
    case 0:
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
      break;

    case 1:
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
      break;

    case 2:
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
      break;

    case 3:
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET);
      break;
  }

}

void ledOff(uint8_t ch)
{
  switch(ch)
  {
    case 0:
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
      break;

    case 1:
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
      break;

    case 2:
      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
      break;

    case 3:
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET);
      break;
  }

}

void ledToggle(uint8_t ch)
{
  switch(ch)
  {
    case 0:
      HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_12);
      break;

    case 1:
      HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
      break;

    case 2:
      HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
      break;

    case 3:
      HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_10);
      break;
  }

}
