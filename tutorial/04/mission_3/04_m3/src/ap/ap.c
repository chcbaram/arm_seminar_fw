/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"






/*
  uint32_t pre_time;

  if (millis()-pre_time >= 100 )
  {
     pre_time = millis();

     // 100ms 마다 실행
  }

  if (buttonGetPressed(0) == true)
  {
         //버튼 눌렸을 때
  }
*/


void apInit(void)
{
}

void apMain(void)
{
  uint8_t button_state;
  uint32_t pre_time;
  uint32_t repeat_cnt;


  uartOpen(_DEF_UART1, 115200);

  button_state = 0;

  while(1)
  {
    // 구현
  }
}
