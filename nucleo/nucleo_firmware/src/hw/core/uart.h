/*
 * uart.h
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#ifndef UART_H_
#define UART_H_



#ifdef __cplusplus
 extern "C" {
#endif


#include "hw_def.h"
#include "bsp.h"




typedef struct
{
   bool     is_open;
   uint8_t  channel;
   uint32_t baud;


   struct
   {
     UART_HandleTypeDef h_uart;
     USART_TypeDef     *h_uart_inst;
     DMA_HandleTypeDef  hdma_rx;
     ring_buf_t         rx_buf;
     bool               dma_enable;
     bool               vcp_enable;
   } hw;

 } uart_t;




bool uartInit(void);


uint32_t    uartOpen(uint8_t channel, uint32_t baud);
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

#endif /* PRJ_SDK_HAL_LED_H_ */
