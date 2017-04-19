#ifndef DEF_H
#define DEF_H


#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>




typedef struct
{
  uint32_t  ptr_in;
  uint32_t  ptr_out;
  uint32_t  length;
  uint8_t  *p_buf;
} ring_buf_t;



typedef uint8_t err_code_t;



#define OK              0

#define _DEF_UART1      0
#define _DEF_UART2      1
#define _DEF_UART3      2






#define ERRCODE_TIMEOUT             255






#endif // DEF_H
