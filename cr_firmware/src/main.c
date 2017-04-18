/*
 * main.c
 *
 *  Created on: 2017. 3. 19.
 *      Author: baram
 */
#include "main.h"



void mainInit(void);





int main(void)
{
  uint32_t t_time;
  uint32_t t_time_us;
  uint32_t t_time_debug;
  uint32_t cnt = 0.;
  uint32_t i;


  mainInit();




  t_time    = millis();
  t_time_us = micros();
  while(1)
  {
    if (millis()-t_time >= 100)
    {
      cnt++;
      t_time    = millis();
      t_time_us = micros();

      for (i=0; i<4; i++)
      {
        if (i == cnt%4)
        {
          ledOn(i);
        }
        else
        {
          ledOff(i);
        }
      }
    }

    if (buttonGetPressed(0) == true)
    {
      if (buttonGetPressedTime(0) > 3000)
      {
        buzzerStart(1000, 100);
        delay(200);
        cmdifLoop();
      }
    }
  }

  return 0;
}

void mainInit(void)
{
  bspInit();
  hwInit();
  apInit();

  cmdifBegin(_DEF_UART1, 115200);
}
