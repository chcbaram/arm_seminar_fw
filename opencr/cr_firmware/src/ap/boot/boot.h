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

#include "bsp.h"
#include "hw.h"



#define BOOT_ERR_INVAILD_CMD        0x00




void bootInit(void);
void bootProcessCmd(cmd_t *p_cmd);
void bootJumpToFw(void);
bool bootCheckFw(void);


#ifdef __cplusplus
}
#endif


#endif
