/*
 *  usb.c
 *
 *
 *  Created on: 2016. 5. 14.
 *      Author: Baram
 */

#include "usb.h"
#include "usb_cdc/usbd_core.h"
#include "usb_cdc/usbd_desc.h"
#include "usb_cdc/usbd_cdc.h"
#include "usb_cdc/usbd_cdc_interface.h"


USBD_HandleTypeDef USBD_Device;






void usbInit(void)
{
  /* Init Device Library */
  USBD_Init(&USBD_Device, &VCP_Desc, 0);

  /* Add Supported Class */
  USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);

  /* Add CDC Interface Class */
  USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);

  /* Start Device Process */
  USBD_Start(&USBD_Device);
}

void usbDeInit(void)
{
  USBD_DeInit(&USBD_Device);
}
