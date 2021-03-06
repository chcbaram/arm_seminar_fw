/*
 * hw.c
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */


#include "hw.h"






void hwInit(void)
{
  bspInit();
  usbInit();
  vcpInit();

  ledInit();
  buttonInit();
  flashInit();
}

void delay(uint32_t time_ms)
{
  HAL_Delay(time_ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}
