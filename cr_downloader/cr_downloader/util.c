/*
 * util.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "util.h"





uint32_t millis(void)
{
  LARGE_INTEGER freq, counter;
  double ret;

  QueryPerformanceCounter(&counter);
  QueryPerformanceFrequency(&freq);

  ret = (double)counter.QuadPart / (double)freq.QuadPart * 1000.0;

  return (uint32_t)ret;
}


void delay(uint32_t delay_ms)
{
  uint32_t time_pre;

  time_pre = millis();
  while(1)
  {
    if (millis()-time_pre >= delay_ms)
    {
      break;
    }
  }
}
