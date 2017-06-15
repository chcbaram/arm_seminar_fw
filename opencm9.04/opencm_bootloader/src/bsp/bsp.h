/*
 *  bsp.h
 *
 *  boart support package
 *
 *  Created on: 2017. 3. 16.
 *      Author: Baram
 */

#ifndef BSP_H
#define BSP_H


#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

#include "def.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

#include "usb_cdc/usbd_core.h"
#include "usb_cdc/usbd_desc.h"
#include "usb_cdc/usbd_cdc.h"
#include "usb_cdc/usbd_cdc_interface.h"







void bspInit();
void bspDeinit();



#ifdef __cplusplus
}
#endif
#endif
