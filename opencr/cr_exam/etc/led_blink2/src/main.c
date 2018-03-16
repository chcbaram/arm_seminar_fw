/*
 * main.c
 *
 *  Created on: 2018. 3. 16.
 *      Author: HanCheol Cho
 */


#include "main.h"




int main(void)
{
  bspInit();
  hwInit();
  apInit();


  apMain();


  return 0;
}
