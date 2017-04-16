/*
 *  boot.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef BOOT_H
#define BOOT_H


#ifdef __cplusplus
 extern "C" {
#endif


#include "def.h"






 typedef struct
 {
   uint32_t tag;
   uint32_t date;
   uint32_t firmware_offset;
   uint32_t firmware_offset_version;
   uint32_t firmware_length;
   uint32_t firmware_check_sum;
 } firmware_tag_t;









bool bootInit(uint8_t channel, char *port_name, uint32_t baud);
bool bootReset(uint8_t channel, char *port_name, uint32_t baud);

err_code_t bootCmdReadVersion(uint8_t *p_version);
err_code_t bootCmdReadBoardName(uint8_t *p_str);

err_code_t bootCmdFlashErase(uint32_t addr, uint32_t length);
err_code_t bootCmdFlashWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
err_code_t bootCmdFlashEraseFw(void);
err_code_t bootCmdFlashWriteFw(uint32_t addr, uint8_t *p_data, uint32_t length);
err_code_t bootCmdFlashWriteTag(firmware_tag_t *p_tag);
err_code_t bootCmdFlashReadTag(firmware_tag_t *p_tag);
err_code_t bootCmdFlashVerfyFw(void);
err_code_t bootCmdJumpToFw(void);

#ifdef __cplusplus
}
#endif


#endif
