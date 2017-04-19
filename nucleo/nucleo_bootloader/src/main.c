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
  mainInit();


  apMain();


  return 0;
}

void mainInit(void)
{
  swtimer_handle_t led_timer_h;
  uint32_t time_pressed;
  uint32_t button_cnt = 0;
  uint8_t  button_num = 1;
  bool boot_excute = false;


  bspInit();
  hwInit();
  apInit();

  led_timer_h = swtimerGetHandle();
  swtimerSet(led_timer_h, 100, LOOP_TIME, led_isr, NULL);


  cmdifBegin(_DEF_UART1, 115200);
  ledOff(0);

  if (wdgGetReset() == false)
  {
    time_pressed = buttonGetPressedTime(button_num);

    while(buttonGetPressed(button_num))
    {
      if (buttonGetPressedTime(button_num)-time_pressed >= 1000)
      {
        time_pressed = buttonGetPressedTime(button_num);
        ledOn(0);
        delay(100);
        ledOff(0);
        button_cnt++;
      }

      if (button_cnt == 3)
      {
        swtimerSet(led_timer_h, 500, LOOP_TIME, led_isr, NULL);
        swtimerStart(led_timer_h);
      }
    }

    if (button_cnt == 1)
    {
      boot_excute = true;
    }

    if (button_cnt == 3)
    {
      cmdifPrint("cmdif begin \r\n");
      cmdifLoop();
      boot_excute = true;

      swtimerSet(led_timer_h, 100, LOOP_TIME, led_isr, NULL);
    }


    if (bootCheckFw() == OK && boot_excute == false)
    {
      bootJumpToFw();
    }
  }

  swtimerStart(led_timer_h);
}
