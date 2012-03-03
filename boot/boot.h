#ifndef BOOT_H_
#define BOOT_H_

#include "common.h"

typedef enum
{
  NORFLASH  = 0,
  NANDFLASH = 1
}BOOTFLASHTYPE;


typedef struct
{
  BOOTFLASHTYPE bootFlashType;
  char fileName[100];
  char * fileBuffer;
  int fileLength;

  INT32 fileEntryPoint;
  INT32 entryPointVa;
}BOOT;


extern BOOT boot;

int loadFromElf(char * fileName, BOOTFLASHTYPE type);
int loadFromBin(char * fileName, BOOTFLASHTYPE type);



#endif
