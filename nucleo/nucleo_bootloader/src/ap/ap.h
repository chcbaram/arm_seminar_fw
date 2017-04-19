/*
 *  ap.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef AP_H
#define AP_H


#ifdef __cplusplus
 extern "C" {
#endif


#include "def.h"

#include "bsp.h"
#include "hw.h"



#include "cmd/cmd.h"
#include "boot/boot.h"


void apInit(void);
void apMain(void);


#ifdef __cplusplus
}
#endif


#endif
