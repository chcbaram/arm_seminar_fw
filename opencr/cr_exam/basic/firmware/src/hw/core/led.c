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

  /* By default, turn off LED */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
}

void ledOn(uint8_t ch)
{
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
}

void ledOff(uint8_t ch)
{
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET);
}

void ledToggle(uint8_t ch)
{
  HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_12);
}
