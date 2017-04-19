/*
 * uart.c
 *
 *  Created on: 2017. 2. 13.
 *      Author: baram
 */
/*
  _HW_DEF_UART_CH_MAX : 2
  _DEF_UART1          : VCP
  _DEF_UART2          : USART2
*/
/*
  USART2
    - RX : PD6, DMA1, Channel 4, Stream 5
    - TX : PD5, DMA1, Channel 4, Stream 6


  USART8
    - RX : PE0, DMA1, Channel 5, Stream 6
    - TX : PE1, DMA1, Channel 5, Stream 0
*/


#include <stdarg.h>
#include <stdbool.h>
#include "hw.h"



#include "uart.h"



#define UART_RX_BUF_LENGTH    64



//-- Internal Variables
//
uart_t  uart_tbl[_HW_DEF_UART_CH_MAX];
uint8_t uart_buffer[2][UART_RX_BUF_LENGTH] __attribute__((section(".NoneCacheableMem")));


//-- External Variables
//


//-- Internal Functions
//
static bool uartIsEnable(uint8_t channel);
void uartStartRx(uint8_t channel);


//-- External Functions
//


//-- Driver Functions
//




bool uartInit(void)
{
  uint32_t i;


  for(i=0; i<_HW_DEF_UART_CH_MAX; i++)
  {
    uart_tbl[i].is_open           = false;
    uart_tbl[i].hw.rx_buf.ptr_in  = 0;
    uart_tbl[i].hw.rx_buf.ptr_out = 0;
    uart_tbl[i].hw.rx_buf.p_buf   = NULL;
    uart_tbl[i].hw.vcp_enable     = false;
    uart_tbl[i].hw.dma_enable     = false;
  }


  return true;
}

uint32_t uartOpen(uint8_t channel, uint32_t baud)
{
	uint32_t err_code  = OK;
	uart_t *p_drv_uart = &uart_tbl[channel];


  if (channel >= _HW_DEF_UART_CH_MAX)
  {
    return 1;
  }



  switch(channel)
  {
    case _DEF_UART1:

      p_drv_uart->baud           = baud;
      p_drv_uart->hw.h_uart_inst = USART2;
      p_drv_uart->hw.dma_enable  = true;
      p_drv_uart->hw.vcp_enable  = false;

      p_drv_uart->hw.rx_buf.ptr_in  = 0;
      p_drv_uart->hw.rx_buf.ptr_out = 0;
      p_drv_uart->hw.rx_buf.p_buf   = (uint8_t *)uart_buffer[0];
      p_drv_uart->hw.rx_buf.length  = UART_RX_BUF_LENGTH;

      p_drv_uart->hw.h_uart.Instance        = p_drv_uart->hw.h_uart_inst;
      p_drv_uart->hw.h_uart.Init.BaudRate   = baud;
      p_drv_uart->hw.h_uart.Init.WordLength = UART_WORDLENGTH_8B;
      p_drv_uart->hw.h_uart.Init.StopBits   = UART_STOPBITS_1;
      p_drv_uart->hw.h_uart.Init.Parity     = UART_PARITY_NONE;
      p_drv_uart->hw.h_uart.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
      p_drv_uart->hw.h_uart.Init.Mode       = UART_MODE_TX_RX;

      HAL_UART_DeInit(&p_drv_uart->hw.h_uart);
      HAL_UART_Init(&p_drv_uart->hw.h_uart);



      p_drv_uart->is_open = true;


      uartStartRx(channel);
      break;
  }





  return err_code;
}

bool uartIsEnable(uint8_t channel)
{
  if (channel >= _HW_DEF_UART_CH_MAX)
  {
    return false;
  }

  return uart_tbl[channel].is_open;
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


  if (p_drv_uart->hw.dma_enable == true)
  {
    p_drv_uart->hw.rx_buf.ptr_in = p_drv_uart->hw.rx_buf.length - p_drv_uart->hw.hdma_rx.Instance->CNDTR;
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


  if (HAL_UART_Transmit(&p_drv_uart->hw.h_uart, (uint8_t*)p_data, length, 100) == HAL_OK)
  {
    ret = length;
  }


  return ret;
}

uint8_t uartRead(uint8_t channel)
{
  uint8_t ret = 0;
  uart_t *p_drv_uart = &uart_tbl[channel];
  uint32_t index;
  volatile uint32_t next_index;


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


//--------------------------------------- Hardware Functions
//
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart_tbl[_DEF_UART2].hw.h_uart);
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  uartErrHandler(uartGetIndex(UartHandle));
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
  uart_t *p_drv_uart;
  uint8_t channel = uartGetIndex(huart);


  p_drv_uart = &uart_tbl[channel];


  if (huart->Instance == USART2)
  {
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    RCC_PeriphCLKInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);


    __HAL_RCC_USART2_CLK_ENABLE();

    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF3_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



    // DMA Setup
    /* Configure the DMA handler for reception process */
    __HAL_RCC_DMA1_CLK_ENABLE();


    p_drv_uart->hw.hdma_rx.Instance                 = DMA1_Channel6;
    p_drv_uart->hw.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    p_drv_uart->hw.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    p_drv_uart->hw.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    p_drv_uart->hw.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    p_drv_uart->hw.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    p_drv_uart->hw.hdma_rx.Init.Mode                = DMA_CIRCULAR;
    p_drv_uart->hw.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    p_drv_uart->hw.hdma_rx.Init.Request             = DMA_REQUEST_2;

    HAL_DMA_Init(&p_drv_uart->hw.hdma_rx);

    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(huart, hdmarx, p_drv_uart->hw.hdma_rx);



    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART2_IRQn, 10, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

  if(huart->Instance==USART2)
  {
    /*##-1- Reset peripherals ##################################################*/
    __HAL_RCC_USART2_FORCE_RESET();
    __HAL_RCC_USART2_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure USARTx Tx as alternate function  */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
    /* Configure USARTx Rx as alternate function  */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    /*##-3- Disable the DMA #####################################################*/
    /* De-Initialize the DMA channel associated to reception process */
    if(huart->hdmarx != 0)
    {
      HAL_DMA_DeInit(huart->hdmarx);
    }
    /* De-Initialize the DMA channel associated to transmission process */
    if(huart->hdmatx != 0)
    {
      HAL_DMA_DeInit(huart->hdmatx);
    }

    HAL_NVIC_DisableIRQ(DMA1_Channel6_IRQn);
  }
}







