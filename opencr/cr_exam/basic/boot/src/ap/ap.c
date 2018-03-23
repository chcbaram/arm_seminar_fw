/*
 * ap.c
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */


#include "ap.h"


void updateLed(void);
void updateButton(void);
void updateDownload(void);

bool runJump(void);
bool runDownload(void);
bool runErase(void);


void apInit(void)
{

}

void apMain(void)
{

  if (buttonGetPressed(0) == false)
  {
    runJump();
  }


  while(1)
  {
    updateLed();
    updateButton();
    updateDownload();
  }
}

void updateLed(void)
{
  static uint32_t pre_time = 0;


  if (millis()-pre_time >= 1000)
  {
    pre_time = millis();
    ledToggle(_HW_DEF_LED_BOOT);
  }
}

void updateButton(void)
{
  if (buttonGetPressed(0) == true)
  {
    ledOn(_DEF_LED2);
  }
  else
  {
    ledOff(_DEF_LED2);
  }

  if (buttonGetPressed(1) == true)
  {
    ledOn(_DEF_LED3);
  }
  else
  {
    ledOff(_DEF_LED3);
  }
}

void updateDownload(void)
{
  if (vcpAvailable() > 0)
  {
    uint8_t ch;

    ch = vcpRead();

    if (ch == 'h')
    {
      vcpPrintf("h.. help\n");
      vcpPrintf("e.. erase\n");
      vcpPrintf("d.. download\n");
      vcpPrintf("j.. jump\n");
    }

    if (ch == 'e')
    {
      runErase();
    }

    if (ch == 'd')
    {
      runDownload();
    }

    if (ch == 'j')
    {
      runJump();
    }
  }
}

bool runJump(void)
{
  bool ret = true;
  void (**jump_func)(void) = (void (**)(void))(0x8040000 + 4);


  if ((uint32_t)(*jump_func) != 0xFFFFFFFF)
  {
    bspDeInit();

    (*jump_func)();
  }
  else
  {
    ret = false;
    vcpPrintf("Jump.. Fail\n");
  }

  return ret;
}

bool runErase(void)
{
  bool ret = true;


  vcpPrintf("Erase..");
  if (flashErase(0x8040000, 256*1024) == true)
  {
    vcpPrintf("OK\n");
  }
  else
  {
    vcpPrintf("Fail\n");
    ret = false;
  }

  return ret;
}

bool runDownload(void)
{
  bool ret = true;
  uint8_t state = 0;
  uint32_t length = 0;
  uint32_t pre_time;
  bool     exit_flag = false;


  vcpPrintf("Download..");

  pre_time = millis();
  while(exit_flag == false)
  {
    switch(state)
    {
      case 0:
        if (vcpAvailable() > 0)
        {
          uint8_t ch;

          ch = vcpRead();
          if (flashWrite(0x8040000, &ch, 1) == true)
          {
            state = 1;
            length++;
            pre_time = millis();
          }
          else
          {
            ret = false;
            exit_flag = true;
          }
        }
        break;

      case 1:
        if (vcpAvailable() > 0)
        {
          uint8_t ch;

          ch = vcpRead();
          if (flashWrite(0x8040000+length, &ch, 1) == true)
          {
            length++;
            pre_time = millis();
          }
          else
          {
            ret = false;
            exit_flag = true;
          }
        }

        if (millis()-pre_time >= 1000)
        {
          exit_flag = true;
        }
        break;
    }
  }

  if (ret == true)
  {
    vcpPrintf("OK");
  }
  else
  {
    vcpPrintf("Fail");
  }

  vcpPrintf(" %d bytes\n", length);

  return ret;
}
