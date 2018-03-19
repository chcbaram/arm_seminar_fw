/*
 * hw.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */

#include "hw.h"


void hwInit(void)
{
  ledInit();
  usbInit();
  vcpInit();
  buttonInit();
  pwmInit();
  timerInit();
  flashInit();
}

uint32_t millis(void)
{
  return HAL_GetTick();
}
