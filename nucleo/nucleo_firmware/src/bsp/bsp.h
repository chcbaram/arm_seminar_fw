/*
 * bsp.h
 *
 *  Created on: 2017. 3. 10.
 *      Author: Baram
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#include <stdbool.h>
#include "stm32l4xx_hal.h"



#ifdef __cplusplus
 extern "C" {
#endif



void bspInit(void);
void bspDeinit();


#ifdef __cplusplus
 }
#endif

#endif /* SRC_BSP_BSP_H_ */
