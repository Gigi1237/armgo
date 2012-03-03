#include <stdio.h>
#include <malloc.h>
#include "memory.h"
#include "s3c2410.h"


MEMORY memory;
extern CPU cpu;


void initMemory(INT32 sdramSize, INT32 flashSize)
{
  memory.sdram_size  = sdramSize;
  memory.sdram_start = (char *)malloc(sdramSize);

  memory.flash_size  = flashSize;
  memory.flash_start = (char *)malloc(flashSize);
}

void freeMemory()
{
  free((void *)memory.sdram_start);
  free((void *)memory.flash_start);
}


char * getBufferAddress(INT32 address)
{
  if( (address >=0 ) && (address <= memory.flash_size ) )
  {
	return (memory.flash_start + address);
  }
  else
  {
	if( (address >= SDRAM_BASE ) && (address < (SDRAM_BASE + SDRAM_SIZE)) )
	{
      return (memory.sdram_start + address - SDRAM_BASE);
	}
	else
	{
      extern int inscount;
	  printf("SDRAM access violate at 0x%08x! ins=0x%08x, inscount=%d, pc=0x%08x\n", address, cpu.pipeline32[0].ins, inscount, cpu.pipeline32[0].address);
	  //return (memory.sdram_start + address - SDRAM_BASE);
	  return (memory.sdram_start);
	}
  }
}


int busRead(INT32 pa, int size, INT32 * data)
{
    pa += MEM_START;

    switch(size)
    {
      case 1:
 	    *data = ( *(INT8 *)(pa) & 0xff );
 	    break;
      case 2:
        *data = ( *(INT16 *)(pa) & 0xffff );
        break;
      case 4:
	    *data = *(INT8 *)(pa);
	    break;
      default:
    	PROGRAM_ERROR;
    	break;
	}
    return 0;
}



int readMem(INT32 address, INT32 * data)
{
  char * ba = getBufferAddress(address);
  *data = *(INT32 *)(ba);
  return 0;
}

int writeMem(INT32 address, INT32 data, int size)
{
  char * ba = getBufferAddress(address);
  int bbit = GETBIT(CP15(1), 7);
  int ebit = GETBIT(CPSR_CUR, 9);
  if(bbit || ebit)
    printf("ERROR....................\n");

  switch(size)
  {
    case 1:
      *(INT8 *)(ba) = (INT8)(data);
      break;
    case 2:
      *(INT16 *)(ba) = (INT16)(data);
      break;
    case 4:
      *(INT32 *)(ba) = (INT32)(data);
      break;
    default:
      PROGRAM_ERROR;
      break;
  }

  return 0;
}

int readMemory(INT32 address, INT32 * data)
{
  /* device port */
  if( (address >= IO_PORT_START) && (address <= IO_PORT_END) )
  {
	*data = readDevicePort(address);
	return 0;
  }
  else
  {
    return readMem(address, data);
  }
}

int writeMemory(INT32 address, INT32 data, int size)
{
  /* device port */
  if( (address >= IO_PORT_START) && (address <= IO_PORT_END) )
  {
	writeDevicePort(address, data);
	return 0;
  }
  else
  {
    return writeMem(address, data, size);
  }
}


INT8 readMemoryByte(INT32 address)
{
  INT32 data;
  if(readMemory(address, &data))
  {
    printf("Read memory exception!\n");
    return 0xff;
  }

  return (INT8)(data & 0xff);
}

INT16 readMemoryHalfword(INT32 address)
{
  INT32 data;
  if(readMemory(address, &data))
  {
    printf("Read memory exception!\n");
    return 0xffff;
  }

  return (INT16)(data & 0xffff);
}



INT32 readMemoryWord(INT32 address)
{
  INT32 data;
  if(readMemory(address, &data))
  {
    printf("Read memory exception!\n");
    return 0xffffffff;
  }

  return (INT32)(data & 0xffffffff);
}


void writeMemoryByte(INT32 address, INT8 data)
{
  if(writeMemory(address, data, 1))
  {
    printf("Write memory exception!\n");
  }
}

void writeMemoryHalfword(INT32 address, INT16 data)
{
  if(writeMemory(address, data, 2))
  {
    printf("Write memory exception!\n");
  }
}

void writeMemoryWord(INT32 address, INT32 data)
{
  if(writeMemory(address, data, 4))
  {
    printf("Write memory exception!\n");
  }
}


mmufault_t readMMU(INT32 va, int size, INT32 * data)
{
	mmufault_t fault;
	tlb_entry_t *tlb;
	//cache_line_t *cache;
	INT32 pa, real_va, temp, offset;


  MMU_VA_TO_MVA(va);
  if(!MMU_ENABLED)
  {
	if(busRead(va, size, data))
	  return 0;
  }

  /* align check */
  if( ( (va & 3) && (size == 4) && MMU_ALIGNED ) ||
	  ( (va & 1) && (size == 2) && MMU_ALIGNED ) )
  {
    return ALIGNMENT_FAULT;
  }

  //va &= ~3;

  /*translate va to tlb */
  fault = translate(va, &memory.mmu.d_tlb, &tlb);
  if(fault)
  {
	return fault;
  }

  /*check access permission */
  fault = checkAccess(va, tlb, 1);
  if(fault)
  {
	return fault;
  }


}
