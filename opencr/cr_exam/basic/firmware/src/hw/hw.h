/*
 * hw.h
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_


#include "hw_def.h"


#include "core/led.h"


void hwInit(void);


void delay(uint32_t time_ms);
uint32_t millis(void);


#endif /* SRC_HW_HW_H_ */
