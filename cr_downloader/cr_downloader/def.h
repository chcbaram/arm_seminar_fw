#ifndef DEF_H
#define DEF_H


#include <stdbool.h>



typedef struct
{
  uint32_t  ptr_in;
  uint32_t  ptr_out;
  uint32_t  length;
  uint8_t  *p_buf;
} ring_buf_t;



#define OK              0

#define _DEF_UART1      0
#define _DEF_UART2      1
#define _DEF_UART3      2



#endif // DEF_H
