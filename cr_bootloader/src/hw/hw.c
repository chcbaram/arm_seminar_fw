/*
 *  hw.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "hw.h"






void hwInit(void)
{
  usbInit();

  ledInit();
  microsInit();
  vcpInit();
  uartInit();
}


