/*
 * led.h
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_CORE_LED_H_
#define SRC_HW_CORE_LED_H_


#include "hw_def.h"


#ifdef __cplusplus
 extern "C" {
#endif

void ledInit(void);

void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);


#ifdef __cplusplus
 }
#endif
#endif /* SRC_HW_CORE_LED_H_ */
