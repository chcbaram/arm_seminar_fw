/*
 * flash.h
 *
 *  Created on: 2018. 3. 22.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_CORE_FLASH_H_
#define SRC_HW_CORE_FLASH_H_

#include "hw_def.h"


#ifdef __cplusplus
 extern "C" {
#endif

void flashInit(void);

bool flashErase(uint32_t addr, uint32_t length);
bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);


#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_CORE_FLASH_H_ */
