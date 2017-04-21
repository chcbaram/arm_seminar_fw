/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"




int demoCmdif(int argc, char **argv);




void apInit(void)
{
}

void apMain(void)
{

  cmdifBegin(_DEF_UART1, 115200);


  cmdifAdd("demo", demoCmdif);

  while(1)
  {
    cmdifLoop();
  }
}

//-- demoCmdif
//
int demoCmdif(int argc, char **argv)
{
  bool ret = true;
  uint32_t time;


  if (argc == 3)
  {
    time = (uint32_t) strtoul((const char * ) argv[2], (char **)NULL, (int) 0);

    if(strcmp("led", argv[1]) == 0)
    {
      while(cmdifRxAvailable() == 0)
      {
        ledToggle(0);
        delay(time);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cmdifPrintf( "demo led time(ms) ...\n");
  }

  return 0;
}

