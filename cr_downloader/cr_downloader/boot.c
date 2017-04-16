/*
 *  boot.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */


#include "boot.h"
#include "cmd.h"
#include "util.h"
#include "uart.h"




#define BOOT_CMD_READ_VERSION           0x00
#define BOOT_CMD_READ_BOARD_NAME        0x01
#define BOOT_CMD_FLASH_ERASE            0x02
#define BOOT_CMD_FLASH_WRITE            0x03
#define BOOT_CMD_FLASH_READ             0x04
#define BOOT_CMD_FLASH_ERASE_FW         0x05
#define BOOT_CMD_FLASH_WRITE_FW         0x06
#define BOOT_CMD_FLASH_WRITE_TAG        0x07
#define BOOT_CMD_FLASH_READ_TAG         0x08
#define BOOT_CMD_FLASH_VERIFY_FW        0x09
#define BOOT_CMD_JUMP_TO_FW             0x010



cmd_t cmd_boot;


uint8_t boot_cmd_buffer[16*1024];



bool bootInit(uint8_t channel, char *port_name, uint32_t baud)
{

  cmdInit(&cmd_boot);

  if (cmdBegin(&cmd_boot, channel, port_name, baud) == false)
  {
    return false;
  }

  return true;
}

bool bootReset(uint8_t channel, char *port_name, uint32_t baud)
{
  uartClose(channel);
  delay(100);
  uartOpen(channel, port_name, 1200);
  uartPutch(channel, 0x03);
  uartClose(channel);uartClose(channel);
  delay(1000);
  uartOpen(channel, port_name, baud);


  return true;
}



err_code_t bootCmdReadVersion(uint8_t *p_version)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;
  uint32_t i;
  uint8_t *data = boot_cmd_buffer;


  data[0] = 1 >> 0;
  data[1] = 2 >> 8;
  data[2] = 3 >> 16;
  data[3] = 4 >> 24;

  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_READ_VERSION, data, 4, 1000);
  if (errcode == OK)
  {
    for (i=0; i<9; i++)
    {
      p_version[i] = p_cmd->rx_packet.data[i];
    }
    p_version[i] = 0;
  }

  return errcode;
}


err_code_t bootCmdReadBoardName(uint8_t *p_str)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;
  uint32_t i;


  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_READ_BOARD_NAME, NULL, 0, 1000);
  if (errcode == OK)
  {
    for (i=0; i<p_cmd->rx_packet.length; i++)
    {
      p_str[i] = p_cmd->rx_packet.data[i];
    }
    p_str[i] = 0;
  }

  return errcode;
}


err_code_t bootCmdFlashErase(uint32_t addr, uint32_t length)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;
  uint8_t data[32];


  data[0] = addr >> 0;
  data[1] = addr >> 8;
  data[2] = addr >> 16;
  data[3] = addr >> 24;

  data[4] = length >> 0;
  data[5] = length >> 8;
  data[6] = length >> 16;
  data[7] = length >> 24;


  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_FLASH_ERASE, data, 8, 3000);

  return errcode;
}


err_code_t bootCmdFlashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;
  uint32_t i;
  uint32_t index;
  uint8_t *data = boot_cmd_buffer;


  data[0] = addr >> 0;
  data[1] = addr >> 8;
  data[2] = addr >> 16;
  data[3] = addr >> 24;

  data[4] = length >> 0;
  data[5] = length >> 8;
  data[6] = length >> 16;
  data[7] = length >> 24;

  index = 8;
  for (i=0; i<length; i++)
  {
    data[index++] = p_data[i];
  }

  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_FLASH_WRITE, data, index, 3000);

  return errcode;
}


err_code_t bootCmdFlashEraseFw(void)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;


  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_FLASH_ERASE_FW, NULL, 0, 5000);

  return errcode;
}


err_code_t bootCmdFlashWriteFw(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;
  uint32_t i;
  uint32_t index;
  uint8_t *data = boot_cmd_buffer;


  data[0] = addr >> 0;
  data[1] = addr >> 8;
  data[2] = addr >> 16;
  data[3] = addr >> 24;

  data[4] = length >> 0;
  data[5] = length >> 8;
  data[6] = length >> 16;
  data[7] = length >> 24;

  index = 8;
  for (i=0; i<length; i++)
  {
    data[index++] = p_data[i];
  }

  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_FLASH_WRITE_FW, data, index, 500);

  return errcode;
}


err_code_t bootCmdFlashWriteTag(firmware_tag_t *p_tag)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;
  uint32_t i;
  uint32_t index;
  uint8_t *data = boot_cmd_buffer;
  uint8_t *p_data = (uint8_t *)p_tag;



  index = 0;
  for (i=0; i< sizeof(firmware_tag_t); i++)
  {
    data[index++] = p_data[i];
  }

  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_FLASH_WRITE_TAG, data, index, 500);

  return errcode;
}


err_code_t bootCmdFlashReadTag(firmware_tag_t *p_tag)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;
  uint32_t i;
  uint8_t *p_data = (uint8_t *)p_tag;


  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_FLASH_READ_TAG, NULL, 0, 500);

  if (errcode == OK)
  {
    for (i=0; i< sizeof(firmware_tag_t); i++)
    {
      p_data[i] = p_cmd->rx_packet.data[i];
    }
  }

  return errcode;
}


err_code_t bootCmdFlashVerfyFw(void)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;


  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_FLASH_VERIFY_FW, NULL, 0, 500);
  return errcode;
}

err_code_t bootCmdJumpToFw(void)
{
  err_code_t errcode = OK;
  cmd_t *p_cmd = &cmd_boot;


  errcode = cmdSendCmdRxResp(p_cmd, BOOT_CMD_JUMP_TO_FW, NULL, 0, 1000);

  return errcode;
}

