#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "boot.h"
#include "memory.h"
#include "elf.h"



BOOT boot;

int bootFromNandFlash()
{
  /* copy first 4K bytes in nand flash to sdram automatically */
  memcpy(memory.flash_start, boot.fileBuffer+boot.fileEntryPoint, 4*1024);
  return 0;
}

int bootFromNorFlash()
{
  /* run directly in nand flash */
  INT32 flashSize = memory.flash_size;
  INT32 copySize  = boot.fileLength - boot.fileEntryPoint;

  if(flashSize < copySize)
  {
	  //flash too small
	  return 1;
  }

  memcpy(memory.flash_start, boot.fileBuffer+boot.fileEntryPoint, copySize);
  return 0;
}


int loadFromBin(char * fileName, BOOTFLASHTYPE type)
{
  FILE * fp;

  if(!(fp = fopen(fileName, "rb")))
  {
	return 1;
  }

  strcpy(boot.fileName, fileName);
  boot.bootFlashType = type;
  boot.fileLength = getFileLength(fp);
  boot.fileBuffer = (char *)malloc(boot.fileLength);
  fread(boot.fileBuffer, 1, boot.fileLength, fp);
  fclose(fp);

  boot.fileEntryPoint = 0;
  boot.entryPointVa   = 0;

  if(type == NORFLASH)
  {
    return(bootFromNorFlash());
  }
  else if(type == NANDFLASH)
  {
    return(bootFromNandFlash());
  }
  else
  {
    return 1;
  }
}


int loadFromElf(char * fileName, BOOTFLASHTYPE type)
{
  FILE * fp;

  if(!(fp = fopen(fileName, "rb")))
  {
	return 1;
  }

  strcpy(boot.fileName, fileName);
  boot.bootFlashType = type;
  boot.fileLength = getFileLength(fp);
  boot.fileBuffer = (char *)malloc(boot.fileLength);
  fread(boot.fileBuffer, 1, boot.fileLength, fp);
  fclose(fp);

  boot.fileEntryPoint = getFileEntryPoint(boot.fileBuffer);
  boot.entryPointVa   = getEntryPointVa(boot.fileBuffer);

  if( (boot.fileEntryPoint == 0xffffffff) || (boot.entryPointVa == 0xffffffff) )
  {
	return 1;
  }

  if(type == NORFLASH)
  {
    return(bootFromNorFlash());
  }
  else if(type == NANDFLASH)
  {
    return(bootFromNandFlash());
  }
  else
  {
    return 1;
  }
}
