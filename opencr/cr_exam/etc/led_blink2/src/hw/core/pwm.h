/*
 * pwm.h
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_CORE_PWM_H_
#define SRC_HW_CORE_PWM_H_


#include "hw_def.h"


void pwmInit(void);


void pwmWrite8(uint8_t pwm_data);


#endif /* SRC_HW_CORE_PWM_H_ */
