/*
 * util.h
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include <Windows.h>


#include "def.h"



#ifdef __cplusplus
 extern "C" {
#endif





uint32_t millis(void);
void delay(uint32_t delay_ms);


#ifdef __cplusplus
}
#endif

#endif
