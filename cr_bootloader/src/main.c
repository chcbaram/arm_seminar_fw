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

#if 0

  mainInit();



  apMain();



#else
  uint32_t t_time;
  uint32_t t_time_us;
  uint32_t cnt = 0.;

  mainInit();






  uartOpen(_DEF_UART1, 115200);
  uartOpen(_DEF_UART2, 115200);


  t_time    = millis();
  t_time_us = micros();
  while(1)
  {
    if (millis()-t_time >= 100)
    {
      vcpPrintf("test %d, %d \r\n", cnt, micros()-t_time_us);
      cnt++;
      t_time    = millis();
      t_time_us = micros();

      ledToggle(cnt%4);

      uartPrintf(_DEF_UART1, "uart1 %d\r\n", cnt);
      uartPrintf(_DEF_UART2, "uart2 %d\r\n", cnt);
    }

    if (vcpAvailable())
    {
      vcpPrintf("Received : %c\r\n", vcpRead());
    }

    if (uartAvailable(_DEF_UART2))
    {
      uartPrintf(_DEF_UART2, "Received : %c\r\n", uartRead(_DEF_UART2));
    }

  }
#endif

  return 0;
}

void mainInit(void)
{
  bspInit();
  hwInit();
  apInit();
}
