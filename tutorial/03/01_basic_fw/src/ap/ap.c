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


  pre_time = millis();
  while(1)
  {
    // 500ms마다 점멸한다.
    //
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(0);
    }
  }
}

