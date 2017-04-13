/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"
#include "boot.h"



#define BOOT_CMD_READ_VERSION       0x00









void bootInit(void)
{
}



void bootCmdReadVersion(cmd_t *p_cmd)
{
  uint8_t data[32] = "B170412R1";



  cmdSendResp(p_cmd, OK, data, 9);
}



void bootProcessCmd(cmd_t *p_cmd)
{
  switch(p_cmd->rx_packet.cmd)
  {
    case BOOT_CMD_READ_VERSION:
      break;

    default:
      cmdSendResp(p_cmd, BOOT_ERR_INVAILD_CMD, NULL, 0);
      break;
  }
}


