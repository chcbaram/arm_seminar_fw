/*
 * hwtimer.h
 *
 *  Created on: 2017. 4. 7.
 *      Author: baram
 */

#ifndef TIMER_H_
#define TIMER_H_



#ifdef __cplusplus
 extern "C" {
#endif


#include "hw_def.h"
#include "bsp.h"






bool hwtimerInit(void);


void hwtimerStop(uint8_t channel);
void hwtimerSetPeriod(uint8_t channel, uint32_t period_data);
void hwtimerAttachInterrupt(uint8_t channel, voidFuncPtr handler);
void hwtimerDetachInterrupt(uint8_t channel);
void hwtimerStart(uint8_t channel);
uint32_t hwtimerGetCounter(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* PRJ_SDK_HAL_LED_H_ */
