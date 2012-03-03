#ifndef MEMORY_H_
#define MEMORY_H_

#include "common.h"
#include "cpu.h"
#include "tlb.h"
#include "cache.h"
#include "wb.h"



typedef struct
{
	tlb_s i_tlb;
	//cache_s i_cache;

	tlb_s d_tlb;
	//cache_s d_cache;
	//wb_s wb_t;
}MMU;


typedef struct MEMORY
{
  //char * start;
  //char * end;
  //INT32 size;

  char * sdram_start;
  INT32 sdram_size;

  char * flash_start;
  INT32 flash_size;
  MMU mmu;

}MEMORY;


extern MEMORY memory;


#define MEM_START ((INT32)memory.sdram_start)
#define MEM_SIZE  ((INT32)memory.sdram_size)


/**********************************************************/
/*virt_addr exchange according to CP15.R13(process id virtul mapping)*/
#define MMU_VA_TO_MVA(va)             \
{                                     \
  if( !(va >> 25) )                   \
  {                                   \
    va |= CP15(13);                   \
  }                                   \
}


#define MMU_ENABLED  (CP15(1)   & CONTROL_MMU)
#define MMU_DISABLED (!(CP15(1) & CONTROL_MMU))
#define MMU_ALIGNED  (CP15(1)   & CONTROL_ALIGN_FAULT)
#define MMU_PAGETABLEBASE (CP15(2) & 0xFFFFC000)



/* Bits in the control register (R1 in CP15) */
#define CONTROL_MMU			        (1<<0)
#define CONTROL_ALIGN_FAULT		    (1<<1)
#define CONTROL_CACHE			    (1<<2)
#define CONTROL_DATA_CACHE		    (1<<2)
#define CONTROL_WRITE_BUFFER		(1<<3)
#define CONTROL_BIG_ENDIAN			(1<<7)
#define CONTROL_SYSTEM				(1<<8)
#define CONTROL_ROM					(1<<9)
#define CONTROL_UNDEFINED           (1<<10)
#define CONTROL_BRANCH_PREDICT      (1<<11)
#define CONTROL_INSTRUCTION_CACHE   (1<<12)
#define CONTROL_VECTOR              (1<<13)
#define CONTROL_RR                  (1<<14)
#define CONTROL_L4                  (1<<15)
#define CONTROL_XP                  (1<<23)
#define CONTROL_EE                  (1<<25)




/* FS[3:0] in the fault status register: */
typedef enum
{
	NO_FAULT = 0x0,
	ALIGNMENT_FAULT = 0x1,
	SECTION_TRANSLATION_FAULT = 0x5,
	PAGE_TRANSLATION_FAULT = 0x7,
	SECTION_DOMAIN_FAULT = 0x9,
	PAGE_DOMAIN_FAULT = 0xB,
	SECTION_PERMISSION_FAULT = 0xD,
	SUBPAGE_PERMISSION_FAULT = 0xF,
}mmufault_t;





#define SDRAM_BASE (0x30000000)
#define SDRAM_SIZE (64*1024*1024)        //64M













/**********************************************************/
void initMemory(INT32 sdramSize, INT32 flashSize);
void freeMemory();

INT8 readMemoryByte(INT32 address);
INT16 readMemoryHalfword(INT32 address);
INT32 readMemoryWord(INT32 address);

void writeMemoryByte(INT32 address, INT8 data);
void writeMemoryHalfword(INT32 address, INT16 data);
void writeMemoryWord(INT32 address, INT32 data);


int busRead(INT32 pa, int size, INT32 * data);

#endif
