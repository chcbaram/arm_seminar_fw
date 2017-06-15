/*
 *  button.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */
#include "hw.h"
#include "button.h"



typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin_number;
  GPIO_PinState on_state;
} button_port_t;

typedef struct
{
  bool        pressed;
  bool        pressed_event;
  uint16_t    pressed_cnt;
  uint32_t    pressed_start_time;
  uint32_t    pressed_end_time;
} button_t;


static button_port_t button_port_tbl[BUTTON_CH_MAX] =
{
    {GPIOC, GPIO_PIN_15,  GPIO_PIN_SET}
};


static button_t button_tbl[BUTTON_CH_MAX];



int buttonCmdif(int argc, char **argv);



void button_isr(void *arg)
{
  uint8_t i;
  GPIO_PinState ret;



  for (i=0; i<BUTTON_CH_MAX; i++)
  {
    ret = HAL_GPIO_ReadPin(button_port_tbl[i].port, button_port_tbl[i].pin_number);

    if (ret == button_port_tbl[i].on_state)
    {
      if (button_tbl[i].pressed == 0)
      {
        button_tbl[i].pressed_start_time = millis();
      }

      button_tbl[i].pressed       = 1;
      button_tbl[i].pressed_event = 1;
      button_tbl[i].pressed_cnt++;

      button_tbl[i].pressed_end_time = millis();
    }
    else
    {
      button_tbl[i].pressed       = 0;
    }
  }
}



bool buttonInit(void)
{
  swtimer_handle_t h_button_timer;
  GPIO_InitTypeDef  GPIO_InitStruct;
  uint32_t i;


  for (i=0; i<BUTTON_CH_MAX; i++)
  {
    GPIO_InitStruct.Pin  = button_port_tbl[i].pin_number;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(button_port_tbl[i].port, &GPIO_InitStruct);

    button_tbl[i].pressed_cnt = 0;
    button_tbl[i].pressed     = 0;
  }


  h_button_timer = swtimerGetHandle();
  swtimerSet(h_button_timer, 1, LOOP_TIME, button_isr, NULL );
  swtimerStart(h_button_timer);


  cmdifAdd("button", buttonCmdif);

  return true;
}

bool buttonGetPressed(uint8_t ch)
{
  bool ret;


  if (ch >= BUTTON_CH_MAX) return false;

  ret = button_tbl[ch].pressed;

  return ret;
}

bool buttonGetPressedEvent(uint8_t ch)
{
  bool ret;


  if (ch >= BUTTON_CH_MAX) return false;

  ret = button_tbl[ch].pressed_event;

  button_tbl[ch].pressed_event = 0;

  return ret;
}

uint32_t buttonGetPressedTime(uint8_t ch)
{
  volatile uint32_t ret;


  if (ch >= BUTTON_CH_MAX) return 0;


  ret = button_tbl[ch].pressed_end_time - button_tbl[ch].pressed_start_time;

  return ret;
}


//-- buttonCmdif
//
int buttonCmdif(int argc, char **argv)
{
  bool ret = true;
  uint8_t number;


  if (argc == 3)
  {
    number = (uint8_t) strtoul((const char * ) argv[2], (char **)NULL, (int) 0);

    if(strcmp("state", argv[1]) == 0)
    {
      while(cmdifRxAvailable() == 0)
      {
        ledToggle(number);
        delay(200);
        cmdifPrintf("button pressed : %d, event : %d, time %d\n",
                    buttonGetPressed(number),
                    buttonGetPressedEvent(number),
                    buttonGetPressedTime(number)
                    );
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
    cmdifPrintf( "button state number ...\n");
  }

  return 0;
}
