/*
 *  download.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */


#include "boot.h"
#include "cmd.h"
#include "util.h"
#include "uart.h"



#define FLASH_FW_SIZE                     (768*1024)  // 768KB
#define FLASH_FW_ADDR_START               0x08040000
#define FLASH_FW_ADDR_END                 (FLASH_FW_ADDR_START + FLASH_FW_SIZE)


#define FLASH_TX_BLOCK_LENGTH	(1024)


static FILE      *opencr_fp;
static uint32_t   opencr_fpsize;






int download(int argc, char *argv[])
{
  uint32_t i;
  uint8_t  version_str[16];
  uint8_t  board_str[16];
  bool ret;
  uint32_t time_pre;
  err_code_t errcode;
  uint32_t baud;
  char *portname;
  uint8_t jump_to_fw = 0;
  uint32_t fw_size = 256*1024*3;

  uint8_t  block_buf[FLASH_TX_BLOCK_LENGTH];
  size_t readbytes = 0;
  bool flash_write_done;
  uint32_t addr;
  uint32_t len;
  uint32_t percent;
  firmware_tag_t fw_tag;
  uint32_t check_sum = 0;



  baud     = strtol( argv[ 2 ], NULL, 10 );
  portname = (char *)argv[ 1 ];

  if( argc >= 5 && strlen(argv[ 4 ])==1 && strncmp(argv[ 4 ], "1", 1)==0 )
  {
    jump_to_fw = 1;
  }

  if( ( opencr_fp = fopen( argv[ 3 ], "rb" ) ) == NULL )
  {
    fprintf( stderr, "Unable to open %s\n", argv[ 3 ] );
    exit( 1 );
  }
  else
  {
    fseek( opencr_fp, 0, SEEK_END );
    opencr_fpsize = ftell( opencr_fp );
    fseek( opencr_fp, 0, SEEK_SET );

    printf("file name \t: %s \n", argv[3]);
    printf("file size \t: %d KB\n", opencr_fpsize/1024);
  }

  fw_size = opencr_fpsize;




  uartInit();


  ret = bootInit(_DEF_UART1, (char *)portname, baud);
  if (ret == true)
  {
    printf("\r\nbootInit \t: OK\n");
    bootReset(_DEF_UART1, (char *)portname, baud);
  }
  else
  {
    printf("\r\nbootInit  Fail\n");
    return 1;
  }


  while(1)
  {
    //-- 버전 확인
    //
    errcode = bootCmdReadVersion(version_str);
    if (errcode == OK)
    {
      printf("boot ver \t: %s\n", version_str);
    }
    else
    {
      printf("bootCmdReadVersion faild \n");
      break;
    }


    //-- 보드 이름 확인
    //
    errcode = bootCmdReadBoardName(board_str);
    if (errcode == OK)
    {
      printf("board name \t: %s\n", board_str);
    }
    else
    {
      printf("bootCmdReadVersion faild \n");
      break;
    }


    //-- Tag 확인
    //
    errcode = bootCmdFlashReadTag(&fw_tag);
    if (errcode == OK)
    {
      printf("\n");
      printf("tag tag \t: 0x%X\n", fw_tag.tag);
      printf("tag fw offset \t: 0x%X\n", fw_tag.firmware_offset);
      printf("tag fw length \t: %dKB (%d bytes)\n", fw_tag.firmware_length/1024, fw_tag.firmware_length);
      printf("tag fw check \t: 0x%X\n", fw_tag.firmware_check_sum);
      printf("\n");
    }
    else
    {
      printf("bootCmdReadVersion faild \n");
      break;
    }



    //-- Flash Erase
    //
    printf("erase fw...\n");
    time_pre = millis();
    errcode = bootCmdFlashEraseFw();
    if (errcode == OK)
    {
      printf("erase fw ret \t: OK (%d ms)\n", millis()-time_pre);
    }
    else
    {
      printf("bootCmdFlashEraseFw faild : %d\n", errcode);
      break;
    }


    //-- Flash Write
    //
    flash_write_done = false;
    addr = 0;
    time_pre = millis();
    while(1)
    {
      if( !feof( opencr_fp ) )
      {
        readbytes = fread( block_buf, 1, FLASH_TX_BLOCK_LENGTH, opencr_fp );
        percent = (addr+readbytes)*100/fw_size;
        printf("flash fw \t: %d %%\r", percent);
      }
      else
      {
        break;
      }

      if( readbytes == 0 )
      {
        break;
      }
      else
      {
        len = readbytes;
      }

      for (int retry=0; retry<3; i++)
      {
        errcode = bootCmdFlashWriteFw(addr, block_buf, len);
        if( errcode == OK )
        {
          break;
        }
      }
      if( errcode != OK )
      {
        break;
      }

      for (i=0; i<len; i++)
      {
        check_sum += block_buf[i];
      }

      addr += len;

      if (addr == fw_size)
      {
        flash_write_done = true;
        break;
      }
    }
    printf("\r\n");

    if( errcode != OK || flash_write_done == false )
    {
      printf("flash fw fail \t: %d\r\n", errcode);
      break;
    }
    else
    {
      printf("flash fw ret \t: OK (%d ms) \r\n", millis()-time_pre);

    }




    //-- Write Tag
    //
    fw_tag.tag  = 0x5555AAAA;
    fw_tag.date = 0x20170401;
    fw_tag.firmware_offset = 0x8040000;
    fw_tag.firmware_length = fw_size;
    fw_tag.firmware_check_sum = check_sum;
    printf("\n");
    printf("write tag...\n");
    time_pre = millis();
    errcode = bootCmdFlashWriteTag(&fw_tag);
    if (errcode == OK)
    {
      printf("write tag ret \t: OK (%d ms)\n", millis()-time_pre);
    }
    else
    {
      printf("write tag faild : %d\n", errcode);
      break;
    }


    //-- Verify Fw
    //
    printf("\n");
    printf("verify fw...\n");
    time_pre = millis();
    errcode = bootCmdFlashVerfyFw();
    if (errcode == OK)
    {
      printf("verify fw ret \t: OK (%d ms)\n", millis()-time_pre);
    }
    else
    {
      printf("verify fw faild : %d\n", errcode);
      break;
    }





    if( errcode != OK || flash_write_done == false )
    {
      printf("Download \t: Fail\r\n");
      return -2;
    }
    else
    {
      printf("Download \t: OK\r\n");
    }


    if (jump_to_fw)
    {
      errcode = bootCmdJumpToFw();
      if (errcode == OK)
      {
        printf("jump to fw \t: OK\n");
      }
      else
      {
        printf("jump to fw fail : %d\n", errcode);
        break;
      }
    }

    break;
  }

  fclose( opencr_fp );
  uartClose(_DEF_UART1);

  return 0;
}
