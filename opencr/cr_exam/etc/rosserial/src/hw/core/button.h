/*
 * button.h
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_CORE_BUTTON_H_
#define SRC_HW_CORE_BUTTON_H_


#include "hw_def.h"

#ifdef __cplusplus
 extern "C" {
#endif

void buttonInit(void);

bool buttonGetPressed(uint8_t ch);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_CORE_BUTTON_H_ */
