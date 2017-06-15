/*
 *  button.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef BUTTON_H
#define BUTTON_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "def.h"
#include "bsp.h"



#define BUTTON_CH_MAX      1





bool buttonInit(void);

bool     buttonGetPressed(uint8_t ch);
uint32_t buttonGetPressedTime(uint8_t ch);


#ifdef __cplusplus
}
#endif


#endif
