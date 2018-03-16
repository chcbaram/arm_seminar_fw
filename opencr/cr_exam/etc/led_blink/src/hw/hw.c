/*
 * hw.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */


#include "hw.h"



void hwInit(void)
{
  delayInit();
  ledInit();
  usbInit();
  vcpInit();

}



uint32_t millis(void)
{
  return HAL_GetTick();
}
