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

      if (ch == 'h')
      {
        vcpPrintf("\n");
        vcpPrintf("h - Help\n");
        vcpPrintf("e - Erase\n");
        vcpPrintf("d - Download\n");
        vcpPrintf("j - Jump\n");
      }

      if (ch == 'e')
      {
        vcpPrintf("Erase..");

        if (flashErase(0x08040000, 256*1024) == true)
        {
          vcpPrintf("OK\n");
        }
        else
        {
          vcpPrintf("Fail\n");
        }
      }

      if (ch == 'j')
      {
        void (**func)(void) = (void (**)(void))(0x08040000 + 4);

        vcpPrintf("jump..");
        delay(100);
        bspDeinit();

        (*func)();
      }

      if (ch == 'd')
      {
        vcpPrintf("Downloading..\n");

        uint32_t recv_length = 0;
        uint32_t pre_time = millis();
        bool     exit_flag = false;
        uint8_t  state = 0;

        while(!exit_flag)
        {
          switch(state)
          {
            case 0:
              if (vcpAvailable() > 0)
              {
                state = 1;
              }
              break;

            case 1:
              if (vcpAvailable() > 0)
              {
                uint8_t ch = vcpRead();


                if (flashWrite(0x08040000+recv_length, &ch, 1) == false)
                {
                  vcpPrintf("FlashWrite Fail : %d\n", recv_length);
                  exit_flag = true;
                }

                recv_length++;

                pre_time = millis();
              }
              if (millis() - pre_time >= 1000)
              {
                vcpPrintf("Length : %d\n", recv_length);
                exit_flag = true;
              }
              break;
          }
        }
      }
    }
  }
}
