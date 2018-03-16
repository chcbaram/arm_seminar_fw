/*
 * ap.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */

#include "ap.h"



void apInit(void)
{

}



void apMain(void)
{
  while(1)
  {
    ledToggle(0);
    delay(1000);
    vcpPrintf("test\n");
  }
}
