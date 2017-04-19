/*
 *  buzzer.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef BUZZER_H
#define BUZZER_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "def.h"
#include "bsp.h"









bool buzzerInit(void);

void buzzerStart(uint32_t frequency, uint32_t duration);
void BuzzerStop(void);


#ifdef __cplusplus
}
#endif


#endif
