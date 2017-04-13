/*
 * uart.cpp
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "uart.h"




uart_t uart_tbl[UART_CH_MAX];




bool uartInit(void)
{
  uint32_t i;


  for(i=0; i<UART_CH_MAX; i++)
  {
    uart_tbl[i].is_open           = false;
  }


  return true;
}

uint32_t uartOpen(uint8_t channel, char *port_name, uint32_t baud)
{
  uint32_t err_code  = OK;
  uart_t *p_uart = &uart_tbl[channel];
  DCB dcb;
  COMMTIMEOUTS timeouts;
  DWORD dwError;


  if (channel >= UART_CH_MAX)
  {
    return 1;
  }


  strcpy(p_uart->port_name, port_name);
  p_uart->baud = baud;




  p_uart->serial_handle = CreateFileA(p_uart->port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (p_uart->serial_handle == INVALID_HANDLE_VALUE)
  {
    printf("Error opening serial port!\n");
    return 2;
  }

  while(1)
  {
    dcb.DCBlength = sizeof(DCB);
    if (GetCommState(p_uart->serial_handle, &dcb) == FALSE)
    {
      err_code = 1;
      break;
    }

    // Set baudrate
    dcb.BaudRate = (DWORD)baud;
    dcb.ByteSize = 8;                    // Data bit = 8bit
    dcb.Parity   = NOPARITY;             // No parity
    dcb.StopBits = ONESTOPBIT;           // Stop bit = 1
    dcb.fParity  = NOPARITY;             // No Parity check
    dcb.fBinary  = 1;                    // Binary mode
    dcb.fNull    = 0;                    // Get Null byte
    dcb.fAbortOnError = 0;
    dcb.fErrorChar    = 0;
    // Not using XOn/XOff
    dcb.fOutX = 0;
    dcb.fInX  = 0;
    // Not using H/W flow control
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fDsrSensitivity = 0;
    dcb.fOutxDsrFlow = 0;
    dcb.fOutxCtsFlow = 0;

    if (SetCommState(p_uart->serial_handle, &dcb) == FALSE)
    {
      err_code = 2;
      break;
    }

    if (SetCommMask(p_uart->serial_handle, 0) == FALSE) // Not using Comm event
    {
      err_code = 3;
      break;
    }
    if (SetupComm(p_uart->serial_handle, 4096, 4096) == FALSE) // Buffer size (Rx,Tx)
    {
      err_code = 4;
      break;
    }
    if (PurgeComm(p_uart->serial_handle, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR) == FALSE) // Clear buffer
    {
      err_code = 5;
      break;
    }
    if (ClearCommError(p_uart->serial_handle, &dwError, NULL) == FALSE)
    {
      err_code = 6;
      break;
    }

    if (GetCommTimeouts(p_uart->serial_handle, &timeouts) == FALSE)
    {
      err_code = 7;
      break;
    }
    // Timeout (Not using timeout)
    // Immediatly return
    timeouts.ReadIntervalTimeout         = 0;
    timeouts.ReadTotalTimeoutMultiplier  = 0;
    timeouts.ReadTotalTimeoutConstant    = 1; // must not be zero.
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant   = 0;
    if (SetCommTimeouts(p_uart->serial_handle, &timeouts) == FALSE)
    {
      err_code = 8;
      break;
    }
    break;
  }

  if (err_code != OK)
  {
    CloseHandle(p_uart->serial_handle);
    printf("err : %x \n", err_code);
  }
  else
  {
    p_uart->is_open = true;
  }

  return err_code;
}

uint32_t uartClose(uint8_t channel)
{
  uint32_t err_code  = OK;
  uart_t *p_uart = &uart_tbl[channel];


  if (channel >= UART_CH_MAX)
  {
    return 1;
  }

  if (p_uart->is_open == true)
  {
    CloseHandle(p_uart->serial_handle);
  }


  return err_code;
}

#if 0
bool uartIsEnable(uint8_t channel)
{
  if (channel >= _HW_DEF_UART_CH_MAX)
  {
    return false;
  }

  if (uart_tbl[channel].hw.vcp_enable == true)
  {
    if (vcpIsConnected() == false || uart_tbl[channel].is_open == false)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  else
  {
    return uart_tbl[channel].is_open;
  }
}

void uartWaitForEnable(uint8_t channel, uint32_t timeout)
{
  uint32_t t_time;


  t_time = millis();
  while(1)
  {
    if (uartIsEnable(channel) == true)
    {
      break;
    }
    if ((millis()-t_time) >= timeout)
    {
      break;
    }
  }
}

uint32_t uartAvailable(uint8_t channel)
{
  uint32_t length = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];


  if (p_drv_uart->hw.vcp_enable == true)
  {
    length = vcpAvailable();
  }
  else
  {
    if (p_drv_uart->hw.dma_enable == true)
    {
      p_drv_uart->hw.rx_buf.ptr_in = p_drv_uart->hw.rx_buf.length - p_drv_uart->hw.hdma_rx.Instance->NDTR;
      length = (   p_drv_uart->hw.rx_buf.length
                 + p_drv_uart->hw.rx_buf.ptr_in
                 - p_drv_uart->hw.rx_buf.ptr_out ) % p_drv_uart->hw.rx_buf.length;

    }
    else
    {
      length = (   p_drv_uart->hw.rx_buf.length
                 + p_drv_uart->hw.rx_buf.ptr_in
                 - p_drv_uart->hw.rx_buf.ptr_out ) % p_drv_uart->hw.rx_buf.length;
    }
  }

  return length;
}

void uartPutch(uint8_t channel, uint8_t ch)
{
  uartWrite(channel, &ch, 1 );
}

uint8_t uartGetch(uint8_t channel)
{
  if (uartIsEnable(channel) == false ) return 0;


  while(1)
  {
    if( uartAvailable(channel) ) break;
  }


  return uartRead(channel);
}

int32_t uartWrite(uint8_t channel, uint8_t *p_data, uint32_t length)
{
  int32_t  ret = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];


  if (uartIsEnable(channel) == false ) return 0;


  if (p_drv_uart->hw.vcp_enable == true)
  {
    ret = vcpWrite(p_data, length);
  }
  else
  {
    if (HAL_UART_Transmit(&p_drv_uart->hw.h_uart, (uint8_t*)p_data, length, 100) == HAL_OK)
    {
      ret = length;
    }
  }


  return ret;
}

uint8_t uartRead(uint8_t channel)
{
  uint8_t ret = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];
  uint32_t index;
  volatile uint32_t next_index;


  if (p_drv_uart->hw.vcp_enable == true)
  {
    ret = vcpRead();
  }
  else
  {
    index      = p_drv_uart->hw.rx_buf.ptr_out;
    next_index = p_drv_uart->hw.rx_buf.ptr_out + 1;

    if (next_index == p_drv_uart->hw.rx_buf.length)
    {
      next_index = 0;;
    }

    if (index != p_drv_uart->hw.rx_buf.ptr_in)
    {
      ret = p_drv_uart->hw.rx_buf.p_buf[index];
      p_drv_uart->hw.rx_buf.ptr_out = next_index;
    }
  }
  return ret;
}

int32_t uartPrintf(uint8_t channel, const char *fmt, ...)
{
  int32_t ret = 0;
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  static char print_buffer[255];


  if (uartIsEnable(channel) == false ) return 0;


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  ret = uartWrite(channel, (uint8_t *)print_buffer, len);

  return ret;
}

int32_t uartPrint(uint8_t channel, uint8_t *p_str)
{
  int32_t index = 0;

  if (uartIsEnable(channel) == false ) return 0;

  while(1)
  {
    uartPutch(channel, p_str[index]);

    if (p_str[index] == 0)
    {
      break;
    }

    index++;

    if (index > 255)
    {
      break;
    }
  }


  return index;
}

void uartStartRx(uint8_t channel)
{
  uart_t *p_drv_uart = &uart_tbl[channel];


  if(p_drv_uart->hw.dma_enable == true)
  {
    HAL_UART_Receive_DMA(&p_drv_uart->hw.h_uart, (uint8_t *)p_drv_uart->hw.rx_buf.p_buf, p_drv_uart->hw.rx_buf.length);
  }
  else
  {
    //
  }

}

void uartErrHandler(uint8_t channel)
{
  uart_t *p_drv_uart = &uart_tbl[channel];


  if(p_drv_uart->hw.dma_enable == false)
  {
      uartStartRx(channel);
  }
}

uint8_t uartGetIndex(UART_HandleTypeDef *UartHandle)
{
  uint32_t i;
  uint8_t  ret = 0;

  for(i=0; i<_HW_DEF_UART_CH_MAX; i++)
  {
    if (UartHandle->Instance == uart_tbl[i].hw.h_uart_inst)
    {
      ret = i;
    }
  }

  return ret;
}


#endif
