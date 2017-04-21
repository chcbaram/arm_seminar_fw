/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"









void apInit(void)
{
}

void apMain(void)
{
  uint32_t pre_time;
  uint32_t cnt = 0;


  uartOpen(_DEF_UART1, 115200);


  pre_time = millis();
  while(1)
  {

    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      uartPrintf(_DEF_UART1, "cnt : %d \n", cnt++);
    }
  }
}

