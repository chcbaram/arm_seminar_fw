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



#define _HW_DEF_BUZZER_CH                 _DEF_TIMER3
#define _HW_DEF_SWTIMER_CH                _DEF_TIMER4



#define _HW_DEF_UART_CH_MAX               2
#define _HW_DEF_SW_TIMER_MAX              8
#define _HW_DEF_CMDIF_LIST_MAX            16
#define _HW_DEF_TIMER_CH_MAX              4





#define FLASH_FW_SIZE                     ((768-32)*1024)   // 736KB
#define FLASH_FW_ADDR_START               0x08040000
#define FLASH_FW_ADDR_END                 (FLASH_FW_ADDR_START + FLASH_FW_SIZE)


#define FLASH_TAG_SIZE                    (32*1024)         // 32KB
#define FLASH_TAG_ADDR_START              FLASH_FW_ADDR_END
#define FLASH_TAG_ADDR_END                (FLASH_TAG_ADDR_START + FLASH_TAG_SIZE)


#define FLASH_FW_VER_ADDR                 0x08040800



#ifdef __cplusplus
}
#endif


#endif
