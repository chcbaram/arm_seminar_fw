/*
 * bsp.h
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */

#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_


#include <stdint.h>
#include <stdbool.h>

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_conf.h"
#include "system_stm32f7xx.h"


void bspInit(void);
void bspDeinit(void);


#endif /* SRC_BSP_BSP_H_ */
