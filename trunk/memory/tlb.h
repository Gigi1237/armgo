#ifndef TLB_H_
#define TLB_H_

#include "common.h"


typedef enum
{
	TLB_INVALID = 0,
	TLB_SMALLPAGE = 1,
	TLB_LARGEPAGE = 2,
	TLB_SECTION = 3,
	TLB_ESMALLPAGE = 4,
	TLB_TINYPAGE = 5
}tlb_mapping_t;


typedef struct
{
  INT32 virt_addr;
  INT32 phys_addr;
  INT32 perms;
  INT32 domain;
  tlb_mapping_t mapping;
}tlb_entry_t;

typedef struct tlb_s
{
  int num;		/*num of tlb entry */
  int cycle;		/*current tlb cycle */
  tlb_entry_t * entrys;
}tlb_s;



int tlbInit(tlb_s * tlb_t, int num);
void tlbExit (tlb_s * tlb_t);
void tlbInvalidateEntry(tlb_s * tlb_t, INT32 addr);
void tlbInvalidateAll(tlb_s * tlb_t);
tlb_entry_t * tlbSearch(tlb_s * tlb_t, INT32 virt_addr);
int checkPerms(int ap, int read);
//mmufault_t checkAccess(INT32 virt_addr, tlb_entry_t * tlb, int read);






#endif
