/*
 * button.h
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_CORE_BUTTON_H_
#define SRC_HW_CORE_BUTTON_H_


#include "hw_def.h"


void buttonInit(void);

bool buttonGetPressed(uint8_t ch);


#endif /* SRC_HW_CORE_BUTTON_H_ */
