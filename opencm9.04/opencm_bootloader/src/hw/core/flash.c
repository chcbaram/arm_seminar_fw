/*
 *  flash.c
 *
 *  Created on: 2016. 7. 13.
 *      Author: Baram
 */
#include "hw.h"
#include "flash.h"



typedef struct
{
  uint32_t addr;
  uint32_t length;
} flash_page_attr_t;



uint32_t            flash_page_total = 128;
uint32_t            flash_page_size  = FLASH_PAGE_SIZE;
flash_page_attr_t   flash_page_attr[128];


int flashCmdif(int argc, char **argv);
static err_code_t flashEraseSector(uint32_t sector);







bool flashInit(void)
{
  uint32_t i;
  uint32_t addr;


  addr = 0x08000000;
  for (i=0; i<flash_page_total; i++)
  {
    flash_page_attr[i].addr   = addr;
    flash_page_attr[i].length = flash_page_size;
    addr += flash_page_size;
  }

  cmdifAdd("flash", flashCmdif);

  return true;
}

err_code_t flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  err_code_t err_code = OK;
  HAL_StatusTypeDef HAL_FLASHStatus = HAL_OK;
  uint32_t StartAddress = addr;
  uint32_t WriteSize;
  uint32_t WriteData;
  uint32_t i;
  uint32_t DataIndex;


  WriteSize = length / 4; // 32Bit

  if( (length%4) > 0 ) WriteSize++;

  DataIndex = 0;
  HAL_FLASH_Unlock();
  for( i=0; i<WriteSize; i++ )
  {
    WriteData  = p_data[ DataIndex++ ] << 0;
    WriteData |= p_data[ DataIndex++ ] << 8;
    WriteData |= p_data[ DataIndex++ ] << 16;
    WriteData |= p_data[ DataIndex++ ] << 24;

    HAL_FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartAddress+i*4, (uint64_t)WriteData);

    if( HAL_FLASHStatus != HAL_OK )
    {
        err_code = ERR_FLASH_WRITE;
      break;
    }
  }
  HAL_FLASH_Lock();

  return err_code;
}


err_code_t flashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  err_code_t err_code = OK;
  uint32_t Dataindex;
  uint32_t addr_cnt;


  Dataindex = 0;
  for (addr_cnt=0;addr_cnt<length;addr_cnt++)
  {
    p_data[Dataindex++] = *(volatile uint8_t*)(addr+addr_cnt);
  }

  return err_code;
}

err_code_t flashErase(uint32_t addr, uint32_t length)
{
  err_code_t err_code = OK;
  uint32_t addr_begin;
  uint32_t addr_end;
  uint32_t target_addr_begin;
  uint32_t target_addr_end;

  uint32_t i;


  target_addr_begin = addr;
  target_addr_end   = addr + length;


  for (i=0; i<flash_page_total; i++)
  {
    addr_begin = flash_page_attr[i].addr;
    addr_end   = flash_page_attr[i].addr + flash_page_attr[i].length - 1;

    if ((addr_begin >= target_addr_begin) && (addr_begin <= target_addr_end))
    {
      err_code = flashEraseSector(i);
    }
    else if((addr_end >= target_addr_begin) && (addr_end <= target_addr_end) )
    {
      err_code = flashEraseSector(i);
    }
  }


  return err_code;
}

err_code_t flashEraseSector(uint32_t sector)
{
  err_code_t err_code = OK;
  HAL_StatusTypeDef HAL_FLASHStatus = HAL_OK;
  FLASH_EraseInitTypeDef pEraseInit;
  uint32_t SectorError;

  pEraseInit.TypeErase    = FLASH_TYPEERASE_PAGES;
  pEraseInit.Banks        = FLASH_BANK_1;
  pEraseInit.PageAddress  = flash_page_attr[sector].addr;
  pEraseInit.NbPages      = 1;


  //-- stm32f103은 Unlock/Lock을 하면 멈춘다.
  HAL_FLASH_Unlock();

  HAL_FLASHStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
  if(HAL_FLASHStatus != HAL_OK)
  {
    err_code = ERR_FLASH_ERASE;
  }

  HAL_FLASH_Lock();

  return err_code;
}

err_code_t flashEraseSectors(uint32_t start_sector, uint32_t sector_cnt )
{
  err_code_t err_code = OK;
  HAL_StatusTypeDef HAL_FLASHStatus = HAL_OK;
  FLASH_EraseInitTypeDef pEraseInit;
  uint32_t SectorError;


  pEraseInit.TypeErase    = FLASH_TYPEERASE_PAGES;
  pEraseInit.Banks        = FLASH_BANK_1;
  pEraseInit.PageAddress  = flash_page_attr[start_sector].addr;
  pEraseInit.NbPages      = sector_cnt;

  HAL_FLASH_Unlock();

  HAL_FLASHStatus = HAL_FLASHEx_Erase(&pEraseInit, &SectorError);
  if(HAL_FLASHStatus != HAL_OK)
  {
    err_code = ERR_FLASH_ERASE;
  }

  HAL_FLASH_Lock();

  return err_code;
}


//-- flashCmdif
//
int flashCmdif(int argc, char **argv)
{
  bool ret = true;
  uint32_t addr;
  uint32_t length;
  err_code_t err_code;
  uint32_t time_pre;
  uint32_t time_process;
  uint32_t data;
  char ver[16];


  if (argc == 2)
  {
    if(strcmp("read_fw_ver", argv[1]) == 0)
    {
      strncpy(ver, (char *)FLASH_FW_VER_ADDR, 9);
      ver[9] = 0;
      cmdifPrintf("fw verison : %s\n", ver);
    }
    else
    {
      ret = false;
    }
  }
  else if (argc == 4)
  {
    addr   = (uint32_t) strtoul((const char * ) argv[2], (char **)NULL, (int) 0);
    length = (uint32_t) strtoul((const char * ) argv[3], (char **)NULL, (int) 0);


    if(strcmp("erase", argv[1]) == 0)
    {
      time_pre = micros();
      err_code = flashErase(addr, length);
      time_process = micros()-time_pre;
      cmdifPrintf("flashErase : %d, %dms\n", err_code, time_process/1000);
    }
    else if(strcmp("write", argv[1]) == 0)
    {
      if (addr%4 != 0)
      {
        cmdifPrintf("addr not aligned 4bytes \n");
      }
      else
      {
        data = length;
        time_pre = micros();
        err_code = flashWrite(addr, (uint8_t *)&data, 4);
        time_process = micros()-time_pre;
        cmdifPrintf("flashWrite : %d, %dus\n", err_code, time_process);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cmdifPrintf( "flash erase addr length \n");
    cmdifPrintf( "flash write addr data   \n");
    cmdifPrintf( "flash read_fw_ver   \n");
  }

  return 0;
}
