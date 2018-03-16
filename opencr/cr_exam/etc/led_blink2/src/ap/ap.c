/*
 * ap.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */


#include "ap.h"



void timerISR(void)
{
  static uint32_t cnt = 0;


  if (cnt%1000 == 0)
  {
    ledToggle(2);
  }

  cnt++;
}




void apInit(void)
{
  timerSetISR(timerISR, 1000);
  timerStart();
}

void apMain(void)
{
  uint32_t pre_time;
  uint32_t pre_time_pwm;
  uint32_t cnt = 0;
  uint8_t  pwm_out = 0;
  int8_t   pwm_dir = 1;

  pre_time = millis();
  pre_time_pwm = millis();
  while(1)
  {
    if (millis()-pre_time >= 1000)
    {
      pre_time = millis();

      ledToggle(0);
      vcpPrintf("test cnt %d\n", cnt++);
    }

    if (buttonGetPressed(0) == true)
    {
      ledOn(1);
    }
    else
    {
      ledOff(1);
    }

    if (millis()-pre_time_pwm >= 10)
    {
      pre_time_pwm = millis();

      pwmWrite8(pwm_out);

      if (pwm_out == 255)
      {
        pwm_dir = -5;
      }
      if (pwm_out == 0)
      {
        pwm_dir = 5;
      }
      pwm_out += pwm_dir;
    }

    if (vcpAvailable() > 0)
    {
      uint8_t ch;

      ch = vcpRead();

      if (ch == '1')
      {
        ledToggle(3);
      }
    }
  }
}
