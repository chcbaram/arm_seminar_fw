/*
 *  micros.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "micros.h"
#include "swtimer.h"













void microsInit()
{
}


uint32_t micros()
{
  return swtimerGetMicroCounter();
}
