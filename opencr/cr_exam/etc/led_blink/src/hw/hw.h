/*
 * hw.h
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_



#include "hw_def.h"


#include "core/delay.h"
#include "core/led.h"
#include "usb_cdc/usb.h"
#include "usb_cdc/vcp.h"


void hwInit(void);


uint32_t millis(void);



#endif /* SRC_HW_HW_H_ */
