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
  uint32_t cnt = 0;


  mainInit();




  t_time    = millis();
  while(1)
  {
    if (millis()-t_time >= 1000)
    {
      t_time = millis();
      uartPrintf(_DEF_UART1, "uartPrint : %d\r\n", cnt++);
    }

    if (buttonGetPressed(0) == true)
    {
      if (buttonGetPressedTime(0) > 3000)
      {
        ledOn(0);
        delay(200);
        ledOff(0);
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
