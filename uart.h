/*
 * uart.h
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <Windows.h>


#include "def.h"



#ifdef __cplusplus
 extern "C" {
#endif




#define UART_CH_MAX    8




typedef struct
{
   bool     is_open;
   uint8_t  channel;
   uint32_t baud;
   HANDLE  serial_handle;
   LARGE_INTEGER freq, counter;
   char     port_name[128];
 } uart_t;




bool uartInit(void);


uint32_t    uartOpen(uint8_t channel, char *port_name, uint32_t baud);
uint32_t    uartClose(uint8_t channel);
uint32_t    uartAvailable(uint8_t channel);
void        uartWaitForEnable(uint8_t channel, uint32_t timeout);
void        uartPutch(uint8_t channel, uint8_t ch);
uint8_t     uartGetch(uint8_t channel);
int32_t     uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length);
uint8_t     uartRead(uint8_t channel);
int32_t     uartPrintf(uint8_t channel, const char *fmt, ...);
int32_t     uartPrint(uint8_t channel, uint8_t *p_str);



#ifdef __cplusplus
}
#endif

#endif
