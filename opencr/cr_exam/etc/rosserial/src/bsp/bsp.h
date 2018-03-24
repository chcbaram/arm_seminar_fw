/*
 * bsp.h
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#include "def.h"


#include "stm32f7xx_hal.h"

#ifdef __cplusplus
 extern "C" {
#endif

void bspInit(void);
void bspDeInit(void);

#ifdef __cplusplus
}
#endif
#endif /* SRC_BSP_BSP_H_ */
