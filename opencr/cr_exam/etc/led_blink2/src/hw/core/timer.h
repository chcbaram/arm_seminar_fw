/*
 * timer.h
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_CORE_TIMER_H_
#define SRC_HW_CORE_TIMER_H_


#include "hw_def.h"


void timerInit(void);


void timerSetISR(void (*func)(), uint32_t time_us);
void timerStart(void);


#endif /* SRC_HW_CORE_TIMER_H_ */
