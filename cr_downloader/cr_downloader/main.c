#include <stdio.h>
#include "uart.h"





int main(int argc, char *argv[])
{
  uint32_t ret;
  uint32_t i;
  uint32_t time_save;

  uartInit();



  ret = uartOpen(_DEF_UART1, (char *)"COM4", 115200);


  if (ret == OK)
  {
    printf("Uart Open OK\n");
  }

  i = 0;
  time_save = millis();
  while(1)
  {
    if (millis()-time_save > 1000)
    {
      time_save = millis();
      i++;

      printf("i %d\n", i);
      uartPutch(_DEF_UART1, 'a');
      if (i > 10) break;
    }

    if (uartAvailable(_DEF_UART1))
    {
      printf("%c", uartRead(_DEF_UART1));
    }


  }

  uartClose(_DEF_UART1);

  return 0;
}
