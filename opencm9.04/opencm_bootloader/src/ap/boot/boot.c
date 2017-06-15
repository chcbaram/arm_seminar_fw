/*
 *  ap.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */

#include "ap.h"
#include "boot.h"



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




typedef struct
{
  uint32_t tag;
  uint32_t date;
  uint32_t firmware_offset;
  uint32_t firmware_offset_version;
  uint32_t firmware_length;
  uint32_t firmware_check_sum;
} firmware_tag_t;



static void bootCmdReadVersion(cmd_t *p_cmd);
static void bootCmdReadBoardName(cmd_t *p_cmd);
static void bootCmdFlashErase(cmd_t *p_cmd);
static void bootCmdFlashEraseFw(cmd_t *p_cmd);
static void bootCmdFlashWrite(cmd_t *p_cmd);
static void bootCmdFlashWriteFw(cmd_t *p_cmd);
static void bootCmdFlashWriteTag(cmd_t *p_cmd);

static void bootCmdJumpToFw(cmd_t *p_cmd);
static void bootCmdFlashVerifyFw(cmd_t *p_cmd);








void bootInit(void)
{
}


void bootJumpToFw(void)
{
  bspDeinit();

  SCB->VTOR = FLASH_FW_ADDR_START;

  __asm volatile("ldr r0, =0x08040000 \n"
                 "ldr sp, [r0]        \n"
                 "ldr pc, [r0, #4]    \n");
}

err_code_t bootCheckFw(void)
{

  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t length;
  uint32_t check_sum = 0;
  uint8_t *p_data;
  uint32_t i;
  firmware_tag_t *p_tag = (firmware_tag_t *)FLASH_TAG_ADDR_START;


  addr_begin = p_tag->firmware_offset;
  addr_end   = p_tag->firmware_offset + p_tag->firmware_length;
  length     = p_tag->firmware_length;


  if ((addr_begin >= FLASH_FW_ADDR_START) && (addr_begin < FLASH_FW_ADDR_END) &&
      (addr_end   >= FLASH_FW_ADDR_START) && (addr_end   < FLASH_FW_ADDR_END))
  {
    if (p_tag->tag == 0x5555AAAA)
    {
      p_data = (uint8_t *)addr_begin;

      for (i=0; i<length; i++)
      {
        check_sum += p_data[i];
      }

      if (check_sum != p_tag->firmware_check_sum)
      {
        err_code = ERR_FLASH_INVALID_CHECK_SUM;
      }
    }
    else
    {
      err_code = ERR_FLASH_INVALID_TAG;
    }
  }
  else
  {
    err_code = ERR_FLASH_INVALID_ADDR;
  }

  return err_code;
}



void bootCmdReadVersion(cmd_t *p_cmd)
{
  uint8_t  data[128] = "B170412R1";
  uint32_t i;
  uint8_t  *p_fw_str = (uint8_t *)FLASH_FW_VER_ADDR;

  for (i=0; i<9; i++)
  {
    data[1*16 + i] = p_fw_str[i];
  }
  data[1*16 + i] = 0;

  cmdSendResp(p_cmd, OK, data, 16*2);
}

void bootCmdReadBoardName(cmd_t *p_cmd)
{
  uint8_t data[32] = "OpenCR R1.0";



  cmdSendResp(p_cmd, OK, data, strlen((char *)data));
}

void bootCmdFlashErase(cmd_t *p_cmd)
{
  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t length;


  addr_begin  = p_cmd->rx_packet.data[0]<<0;
  addr_begin |= p_cmd->rx_packet.data[1]<<8;
  addr_begin |= p_cmd->rx_packet.data[2]<<16;
  addr_begin |= p_cmd->rx_packet.data[3]<<24;

  length      = p_cmd->rx_packet.data[4]<<0;
  length     |= p_cmd->rx_packet.data[5]<<8;
  length     |= p_cmd->rx_packet.data[6]<<16;
  length     |= p_cmd->rx_packet.data[7]<<24;

  addr_end    = addr_begin + length - 1;


  if ((addr_begin >= FLASH_FW_ADDR_START) && (addr_begin < FLASH_FW_ADDR_END) &&
      (addr_end   >= FLASH_FW_ADDR_START) && (addr_end   < FLASH_FW_ADDR_END))
  {
    if (addr_begin%4 == 0)
    {
      err_code = flashErase(addr_begin, length);
    }
    else
    {
      err_code = ERR_FLASH_ADDR_ALIGN;
    }
  }
  else
  {
    err_code = ERR_FLASH_INVALID_ADDR;
  }

  cmdSendResp(p_cmd, err_code, NULL, 0);

}

void bootCmdFlashWrite(cmd_t *p_cmd)
{
  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t length;


  addr_begin  = p_cmd->rx_packet.data[0]<<0;
  addr_begin |= p_cmd->rx_packet.data[1]<<8;
  addr_begin |= p_cmd->rx_packet.data[2]<<16;
  addr_begin |= p_cmd->rx_packet.data[3]<<24;

  length      = p_cmd->rx_packet.data[4]<<0;
  length     |= p_cmd->rx_packet.data[5]<<8;
  length     |= p_cmd->rx_packet.data[6]<<16;
  length     |= p_cmd->rx_packet.data[7]<<24;

  addr_end    = addr_begin + length - 1;


  if ((addr_begin >= FLASH_FW_ADDR_START) && (addr_begin < FLASH_FW_ADDR_END) &&
      (addr_end   >= FLASH_FW_ADDR_START) && (addr_end   < FLASH_FW_ADDR_END))
  {
    if (addr_begin%4 == 0)
    {
      err_code = flashWrite(addr_begin, &p_cmd->rx_packet.data[8], length);
    }
    else
    {
      err_code = ERR_FLASH_ADDR_ALIGN;
    }
  }
  else
  {
    err_code = ERR_FLASH_INVALID_ADDR;
  }

  cmdSendResp(p_cmd, err_code, NULL, 0);
}

void bootCmdFlashEraseFw(cmd_t *p_cmd)
{
  err_code_t err_code = OK;

  err_code = flashErase(FLASH_FW_ADDR_START, FLASH_FW_SIZE);

  cmdSendResp(p_cmd, err_code, NULL, 0);
}

void bootCmdFlashWriteFw(cmd_t *p_cmd)
{
  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t length;


  addr_begin  = p_cmd->rx_packet.data[0]<<0;
  addr_begin |= p_cmd->rx_packet.data[1]<<8;
  addr_begin |= p_cmd->rx_packet.data[2]<<16;
  addr_begin |= p_cmd->rx_packet.data[3]<<24;

  length      = p_cmd->rx_packet.data[4]<<0;
  length     |= p_cmd->rx_packet.data[5]<<8;
  length     |= p_cmd->rx_packet.data[6]<<16;
  length     |= p_cmd->rx_packet.data[7]<<24;


  addr_begin  = addr_begin + FLASH_FW_ADDR_START;
  addr_end    = addr_begin + length - 1;


  if ((addr_begin >= FLASH_FW_ADDR_START) && (addr_begin < FLASH_FW_ADDR_END) &&
      (addr_end   >= FLASH_FW_ADDR_START) && (addr_end   < FLASH_FW_ADDR_END))
  {
    if (addr_begin%4 == 0)
    {
      err_code = flashWrite(addr_begin, &p_cmd->rx_packet.data[8], length);
    }
    else
    {
      err_code = ERR_FLASH_ADDR_ALIGN;
    }
  }
  else
  {
    err_code = ERR_FLASH_INVALID_ADDR;
  }

  cmdSendResp(p_cmd, err_code, NULL, 0);
}


void bootCmdFlashWriteTag(cmd_t *p_cmd)
{
  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t length;



  length      = sizeof(firmware_tag_t);
  addr_begin  = FLASH_TAG_ADDR_START;
  addr_end    = addr_begin + length - 1;


  if ((addr_begin >= FLASH_TAG_ADDR_START) && (addr_begin < FLASH_TAG_ADDR_END) &&
      (addr_end   >= FLASH_TAG_ADDR_START) && (addr_end   < FLASH_TAG_ADDR_END))
  {
    if (addr_begin%4 == 0)
    {
      err_code = flashWrite(addr_begin, &p_cmd->rx_packet.data[0], length);
    }
    else
    {
      err_code = ERR_FLASH_ADDR_ALIGN;
    }
  }
  else
  {
    err_code = ERR_FLASH_INVALID_ADDR;
  }

  cmdSendResp(p_cmd, err_code, NULL, 0);
}

void bootCmdFlashReadTag(cmd_t *p_cmd)
{
  err_code_t err_code = OK;
  uint8_t *p_data;
  firmware_tag_t tag;


  p_data = (uint8_t *)&tag;
  tag = *(firmware_tag_t *)FLASH_TAG_ADDR_START;

  tag.firmware_offset = FLASH_FW_ADDR_START;

  cmdSendResp(p_cmd, err_code, p_data, sizeof(firmware_tag_t));
}


void bootCmdFlashVerifyFw(cmd_t *p_cmd)
{
  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t length;
  uint32_t check_sum = 0;
  uint8_t *p_data;
  uint32_t i;
  firmware_tag_t *p_tag = (firmware_tag_t *)FLASH_TAG_ADDR_START;


  addr_begin = p_tag->firmware_offset;
  addr_end   = p_tag->firmware_offset + p_tag->firmware_length;
  length     = p_tag->firmware_length;


  if ((addr_begin >= FLASH_FW_ADDR_START) && (addr_begin < FLASH_FW_ADDR_END) &&
      (addr_end   >= FLASH_FW_ADDR_START) && (addr_end   < FLASH_FW_ADDR_END))
  {
    if (p_tag->tag == 0x5555AAAA)
    {
      p_data = (uint8_t *)addr_begin;

      for (i=0; i<length; i++)
      {
        check_sum += p_data[i];
      }

      if (check_sum != p_tag->firmware_check_sum)
      {
        err_code = ERR_FLASH_INVALID_CHECK_SUM;
      }
    }
    else
    {
      err_code = ERR_FLASH_INVALID_TAG;
    }
  }
  else
  {
    err_code = ERR_FLASH_INVALID_ADDR;
  }


  cmdSendResp(p_cmd, err_code, NULL, 0);
}

void bootCmdJumpToFw(cmd_t *p_cmd)
{
  if (bootCheckFw() == OK)
  {
    cmdSendResp(p_cmd, OK, NULL, 0);
    delay(100);
    bootJumpToFw();
  }
  else
  {
    cmdSendResp(p_cmd, ERR_INVALID_FW, NULL, 0);
  }
}


void bootProcessCmd(cmd_t *p_cmd)
{
  switch(p_cmd->rx_packet.cmd)
  {
    case BOOT_CMD_READ_VERSION:
      bootCmdReadVersion(p_cmd);
      break;

    case BOOT_CMD_READ_BOARD_NAME:
      bootCmdReadBoardName(p_cmd);
      break;

    case BOOT_CMD_FLASH_ERASE:
      bootCmdFlashErase(p_cmd);
      break;

    case BOOT_CMD_FLASH_WRITE:
      bootCmdFlashWrite(p_cmd);
      break;

    case BOOT_CMD_FLASH_ERASE_FW:
      bootCmdFlashEraseFw(p_cmd);
      break;

    case BOOT_CMD_FLASH_WRITE_FW:
      bootCmdFlashWriteFw(p_cmd);
      break;

    case BOOT_CMD_FLASH_WRITE_TAG:
      bootCmdFlashWriteTag(p_cmd);
      break;

    case BOOT_CMD_FLASH_READ_TAG:
      bootCmdFlashReadTag(p_cmd);
      break;


    case BOOT_CMD_FLASH_VERIFY_FW:
      bootCmdFlashVerifyFw(p_cmd);
      break;

    case BOOT_CMD_JUMP_TO_FW:
      bootCmdJumpToFw(p_cmd);
      break;


    default:
      cmdSendResp(p_cmd, BOOT_ERR_INVAILD_CMD, NULL, 0);
      break;
  }
}


