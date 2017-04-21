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
} flash_sector_attr_t;



uint32_t            flash_sector_total = FLASH_SECTOR_TOTAL;
flash_sector_attr_t flash_sector_attr[FLASH_SECTOR_TOTAL];


int flashCmdif(int argc, char **argv);
static err_code_t flashEraseSector(uint32_t sector);







bool flashInit(void)
{
  flash_sector_attr[0].addr   = 0x08000000;
  flash_sector_attr[0].length = 32*1024;

  flash_sector_attr[1].addr   = 0x08008000;
  flash_sector_attr[1].length = 32*1024;

  flash_sector_attr[2].addr   = 0x08010000;
  flash_sector_attr[2].length = 32*1024;

  flash_sector_attr[3].addr   = 0x08018000;
  flash_sector_attr[3].length = 32*1024;

  flash_sector_attr[4].addr   = 0x08020000;
  flash_sector_attr[4].length = 128*1024;

  flash_sector_attr[5].addr   = 0x08040000;
  flash_sector_attr[5].length = 256*1024;

  flash_sector_attr[6].addr   = 0x08080000;
  flash_sector_attr[6].length = 256*1024;

  flash_sector_attr[7].addr   = 0x080C0000;
  flash_sector_attr[7].length = 256*1024;


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


  for (i=0; i<FLASH_SECTOR_TOTAL; i++)
  {
    addr_begin = flash_sector_attr[i].addr;
    addr_end   = flash_sector_attr[i].addr + flash_sector_attr[i].length - 1;

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

  pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
  pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  pEraseInit.Sector       = sector;
  pEraseInit.NbSectors    = 1;

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


  pEraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
  pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  pEraseInit.Sector       = start_sector;
  pEraseInit.NbSectors    = sector_cnt;

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
