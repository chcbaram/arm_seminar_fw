/*
 *  hw_def.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef HW_DEF_H
#define HW_DEF_H


#ifdef __cplusplus
 extern "C" {
#endif




#define _HW_DEF_SWTIMER_CH                _DEF_TIMER1



#define _HW_DEF_UART_CH_MAX               2
#define _HW_DEF_SW_TIMER_MAX              8
#define _HW_DEF_CMDIF_LIST_MAX            16
#define _HW_DEF_TIMER_CH_MAX              4





#define FLASH_FW_SIZE                     ((64-2)*1024)   // 64KB
#define FLASH_FW_ADDR_START               0x08008000
#define FLASH_FW_ADDR_END                 (FLASH_FW_ADDR_START + FLASH_FW_SIZE)


#define FLASH_TAG_SIZE                    (2*1024)         // 2KB
#define FLASH_TAG_ADDR_START              FLASH_FW_ADDR_END
#define FLASH_TAG_ADDR_END                (FLASH_TAG_ADDR_START + FLASH_TAG_SIZE)


#define FLASH_FW_VER_ADDR                 0x08008800



#ifdef __cplusplus
}
#endif


#endif
