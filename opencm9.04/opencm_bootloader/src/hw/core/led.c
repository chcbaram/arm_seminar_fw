/*
 *  led.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */
#include "hw.h"
#include "led.h"



typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin_number;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_port_t;


static led_port_t led_port_tbl[LED_CH_MAX] =
{
    {GPIOB, GPIO_PIN_9, GPIO_PIN_RESET, GPIO_PIN_SET}
};



int ledCmdif(int argc, char **argv);




void ledInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  uint32_t i;


  for (i=0; i<LED_CH_MAX; i++)
  {
    GPIO_InitStruct.Pin   = led_port_tbl[i].pin_number;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(led_port_tbl[i].port, &GPIO_InitStruct);

    ledOff(i);
  }


  cmdifAdd("led", ledCmdif);
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  HAL_GPIO_WritePin(led_port_tbl[ch].port, led_port_tbl[ch].pin_number, led_port_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  HAL_GPIO_WritePin(led_port_tbl[ch].port, led_port_tbl[ch].pin_number, led_port_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_CH_MAX) return;

  HAL_GPIO_TogglePin(led_port_tbl[ch].port, led_port_tbl[ch].pin_number);
}



//-- ledCmdif
//
int ledCmdif(int argc, char **argv)
{
  bool ret = true;
  uint8_t number;


  if (argc == 3)
  {
    number = (uint8_t) strtoul((const char * ) argv[2], (char **)NULL, (int) 0);

    if(strcmp("on", argv[1]) == 0)
    {
      ledOn(number);
    }
    else if(strcmp("off", argv[1])==0)
    {
      ledOff(number);
    }
    else if(strcmp("toggle", argv[1])==0)
    {
      ledToggle(number);
    }
    else if(strcmp("demo", argv[1])==0)
    {
      while(cmdifRxAvailable() == 0)
      {
        ledToggle(number);
        delay(200);
        cmdifPrintf("led toggle %d\n", number);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cmdifPrintf( "led on/off/toggle/demo number ...\n");
  }

  return 0;
}
