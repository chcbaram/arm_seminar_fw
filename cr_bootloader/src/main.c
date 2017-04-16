/*
 * main.c
 *
 *  Created on: 2017. 3. 19.
 *      Author: baram
 */
#include "main.h"



void mainInit(void);



void led_isr(void *arg)
{
  ledToggle(0);
}



int main(void)
{


#if 1

  mainInit();





  //apMain();

  while(1)
  {
    cmdifMain();
  }


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

      //ledToggle(cnt%4);

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
  swtimer_handle_t led_timer_h;
  uint32_t time_pressed;
  uint32_t button_cnt = 0;
  uint8_t  button_num = 1;

  bspInit();
  hwInit();
  apInit();

  led_timer_h = swtimerGetHandle();
  swtimerSet(led_timer_h, 500, LOOP_TIME, led_isr, NULL);
  swtimerStart(led_timer_h);

  cmdifBegin(_DEF_UART1, 115200);


  time_pressed = buttonGetPressedTime(button_num);

  while(buttonGetPressed(button_num))
  {
    if (button_cnt == 3)
    {
      cmdifPrint("cmdif begin \r\n");
      cmdifLoop();
      break;
    }

    if (buttonGetPressedTime(button_num)-time_pressed >= 1000)
    {
      time_pressed = buttonGetPressedTime(button_num);
      buzzerStart(1000, 100);
      button_cnt++;
    }
  }
}
