/*
 *  delay.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef DELAY_H
#define DELAY_H


#ifdef __cplusplus
 extern "C" {
#endif


#include "def.h"
#include "bsp.h"




#define delay(x)  delay_ms(x)



void delay_ns(uint32_t ns);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);




#ifdef __cplusplus
}
#endif


#endif
