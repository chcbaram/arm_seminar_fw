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




  while(1)
  {
    cmdifLoop();
  }
}
