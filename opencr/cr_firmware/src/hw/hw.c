/*
 *  hw.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "hw.h"






void hwInit(void)
{
  cmdifInit();
  swtimerInit();
  hwtimerInit();
  usbInit();
  ledInit();
  microsInit();
  vcpInit();
  uartInit();
  buttonInit();
  buzzerInit();
  flashInit();

  // 1ms for swtimer
  //
  hwtimerSetPeriod(_HW_DEF_SWTIMER_CH, 1000);
  hwtimerAttachInterrupt(_HW_DEF_SWTIMER_CH, swtimerISR);
  hwtimerStart(_HW_DEF_SWTIMER_CH);
}


