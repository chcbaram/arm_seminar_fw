/*
 * main.c
 *
 *  Created on: 2017. 3. 19.
 *      Author: baram
 */
/*
 for OpenOCD
 -f opencr_openocd.cfg
 */
#include "main.h"



void mainInit(void);






int main(void)
{
  mainInit();


  apMain();

  return 0;
}

void mainInit(void)
{
  bspInit();
  hwInit();
  apInit();
}
