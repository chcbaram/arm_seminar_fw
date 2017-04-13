#include <stdio.h>
#include "uart.h"





int main(int argc, char *argv[])
{
  uint32_t ret;


  uartInit();



  ret = uartOpen(_DEF_UART1, (char *)"COM4", 115200);


  if (ret == OK)
  {
    printf("Uart Open OK\n");
  }

  uartClose(_DEF_UART1);

  return 0;
}
