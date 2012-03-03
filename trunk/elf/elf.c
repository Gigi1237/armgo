#include <linux/elf.h>
#include "elf.h"


INT32 getFileEntryPoint(char * buffer)
{
  int i;
  Elf32_Ehdr * elfHeader     = (Elf32_Ehdr *)buffer;
  Elf32_Phdr * programHeader = (Elf32_Phdr *)(buffer + elfHeader->e_phoff);

  printf("0x%08x\n", buffer[0]);
  for(i=0; i<elfHeader->e_phnum; ++i)
  {
	printf("type = %d\n", programHeader[i].p_type);
    if(programHeader[i].p_type == PT_LOAD)
      return programHeader[i].p_offset;
  }

  return 0xffffffff;
}

INT32 getEntryPointVa(char * buffer)
{
  int i;
  Elf32_Ehdr * elfHeader     = (Elf32_Ehdr *)buffer;
  Elf32_Phdr * programHeader = (Elf32_Phdr *)(buffer + elfHeader->e_phoff);

  for(i=0; i<elfHeader->e_phnum; ++i)
  {
    if(programHeader[i].p_type == PT_LOAD)
      return programHeader[i].p_paddr;
  }

  return 0xffffffff;
}

INT32 getFileLength(FILE * fp)
{
  int length;

  fseek(fp, 0, SEEK_END);
  length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  return length;
}
