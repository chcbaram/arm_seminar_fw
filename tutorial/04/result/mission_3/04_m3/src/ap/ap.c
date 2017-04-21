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
  uint8_t button_state;
  uint32_t pre_time;
  uint32_t repeat_cnt;


  uartOpen(_DEF_UART1, 115200);

  button_state = 0;

  while(1)
  {
    switch(button_state)
    {
      case 0:
        if (buttonGetPressed(0) == true)
        {
          pre_time = millis();
          button_state = 1;
        }
        break;

      case 1:
        if (buttonGetPressed(0) == false)
        {
          button_state = 0;
        }
        if (millis()-pre_time >= 30 )
        {
          uartPrintf(_DEF_UART1, "start\n");
          button_state = 2;
          pre_time = millis();
        }
        break;

      case 2:
        if (buttonGetPressed(0) == false)
        {
          button_state = 0;
        }
        if (millis()-pre_time >= 1000 )
        {
          uartPrintf(_DEF_UART1, "repeat_start\n");
          button_state = 3;
          repeat_cnt   = 0;
          pre_time = millis();
        }
        break;

      case 3:
        if (buttonGetPressed(0) == false)
        {
          button_state = 4;
        }
        if (millis()-pre_time >= 100 )
        {
          repeat_cnt++;
          uartPrintf(_DEF_UART1, "repeat %d\n", repeat_cnt);
          pre_time = millis();
        }
        break;

      case 4:
        uartPrintf(_DEF_UART1, "end\n");
        button_state = 0;
        break;

    }
  }
}

