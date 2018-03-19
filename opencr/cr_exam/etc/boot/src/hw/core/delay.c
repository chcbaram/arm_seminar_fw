/*
 * delay.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */


#include "delay.h"



void delayInit(void)
{

}

void delay(uint32_t time_ms)
{
  HAL_Delay(time_ms);
}
