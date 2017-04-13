/*
 *  def.h
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#ifndef DEF_H
#define DEF_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#ifndef BOOL
#define BOOL uint8_t
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define OK    0



#include "hw_def.h"
#include "ap_def.h"



#define _DEF_UART1          0
#define _DEF_UART2          1
#define _DEF_UART3          2
#define _DEF_UART4          3
#define _DEF_UART5          4
#define _DEF_UART6          5


#define _DEF_INPUT          0
#define _DEF_OUPUT          1

#define _DEF_HIGH           1
#define _DEF_LOW            0




typedef struct
{
  uint32_t  ptr_in;
  uint32_t  ptr_out;
  uint32_t  length;
  uint8_t  *p_buf;
} ring_buf_t;




typedef void (*voidFuncPtr)(void);













#endif
