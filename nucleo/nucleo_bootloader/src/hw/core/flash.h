/*
 *  flash.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef FLASH_H
#define FLASH_H


#ifdef __cplusplus
 extern "C" {
#endif



#include "def.h"
#include "bsp.h"








bool flashInit(void);

err_code_t flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
err_code_t flashRead(uint32_t addr, uint8_t *p_data, uint32_t length);
err_code_t flashErase(uint32_t addr, uint32_t length);
err_code_t flashEraseSectors(uint32_t start_sector, uint32_t sector_cnt );


#ifdef __cplusplus
}
#endif


#endif
