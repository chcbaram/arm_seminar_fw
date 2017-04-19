/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"




cmd_t cmd_boot;




void apInit(void)
{
}

void apMain(void)
{

  cmdInit(&cmd_boot);
  cmdBegin(&cmd_boot, _DEF_UART1, 115200);


  while(1)
  {
    if (cmdReceivePacket(&cmd_boot) == true)
    {
      bootProcessCmd(&cmd_boot);
    }
  }
}

