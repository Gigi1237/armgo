
#include <malloc.h>
#include "memory.h"
#include "tlb.h"
#include "cpu.h"



INT32 tlb_masks[] =
{
	0x00000000,		/* TLB_INVALID */
	0xFFFFF000,		/* TLB_SMALLPAGE */
	0xFFFF0000,		/* TLB_LARGEPAGE */
	0xFFF00000,		/* TLB_SECTION */
	0xFFFFF000,		/*TLB_ESMALLPAGE, have TEX attirbute, only for XScale */
	0xFFFFFC00		/* TLB_TINYPAGE */
};


int tlbInit(tlb_s * tlb_t, int num)
{
	tlb_entry_t * e;
	int i;

	e = (tlb_entry_t *)malloc(sizeof(tlb_entry_t) * num);

	tlb_t->entrys = e;
	for(i = 0; i < num; i++, e++)
	  e->mapping = TLB_INVALID;

	tlb_t->cycle = 0;
	tlb_t->num = num;

	return 0;
}


void tlbExit (tlb_s * tlb_t)
{
  free(tlb_t->entrys);
};



void tlbInvalidateEntry(tlb_s * tlb_t, INT32 addr)
{
	tlb_entry_t * tlb;

	tlb = tlbSearch(tlb_t, addr);
	if(tlb)
	{
		tlb->mapping = TLB_INVALID;
	}
}


void tlbInvalidateAll(tlb_s * tlb_t)
{
	int entry;

	for(entry = 0; entry < tlb_t->num; entry++)
	{
		tlb_t->entrys[entry].mapping = TLB_INVALID;
	}
	tlb_t->cycle = 0;
}



tlb_entry_t * tlbSearch(tlb_s * tlb_t, INT32 virt_addr)
{
	int entry;

	for(entry = 0; entry < tlb_t->num; entry++)
	{
		tlb_entry_t * tlb;
		INT32 mask;

		tlb = &(tlb_t->entrys[entry]);
		if(tlb->mapping == TLB_INVALID)
		{
			continue;
		}
		mask = tlb_masks[tlb->mapping];
		if((virt_addr & mask) == (tlb->virt_addr & mask))
		{
			return tlb;
		}
	}
	return NULL;
}



mmufault_t translate(INT32 virt_addr, tlb_s * tlb_t, tlb_entry_t ** tlb)
{
	*tlb = tlbSearch(tlb_t, virt_addr);

	if(!*tlb)
	{
		/* walk the translation tables */
		INT32 l1addr, l1desc;
		tlb_entry_t entry;

		l1addr = MMU_PAGETABLEBASE;
		l1addr = ( l1addr | (virt_addr >> 18) ) & ~3;
		busRead(4, l1addr, &l1desc);
		switch (l1desc & 3)
		{
		  case 0:
			/*
			 * according to Figure 3-9 Sequence for checking faults in arm manual,
			 * section translation fault should be returned here.
			 */
			{
				return SECTION_TRANSLATION_FAULT;
			}
		  case 3:
			/* fine page table */
			{
				INT32 l2addr, l2desc;

				l2addr = l1desc & 0xFFFFF000;
				l2addr = ( l2addr | ( (virt_addr & 0x000FFC00) >> 8) ) & ~3;
				// second page table
				busRead(4, l2addr, &l2desc);

				entry.virt_addr = virt_addr;
				entry.phys_addr = l2desc;
				entry.perms = l2desc & 0x00000FFC;
				entry.domain = (l1desc >> 5) & 0x0000000F;
				switch(l2desc & 3)
				{
				  case 0:
					return PAGE_TRANSLATION_FAULT;
				  case 3:
					entry.mapping = TLB_TINYPAGE;
					break;
				  case 1:
					// this is untested
					entry.mapping = TLB_LARGEPAGE;
					break;
				  case 2:
					// this is untested
					entry.mapping = TLB_SMALLPAGE;
					break;
				}
			}
			break;
		  case 1:
			/* coarse page table */
			{
				INT32 l2addr, l2desc;

				l2addr = l1desc & 0xFFFFFC00;
				l2addr = (l2addr | ( (virt_addr & 0x000FF000) >> 10) ) & ~3;
				busRead(4, l2addr, &l2desc);

				entry.virt_addr = virt_addr;
				entry.phys_addr = l2desc;
				entry.perms = l2desc & 0x00000FFC;
				entry.domain = (l1desc >> 5) & 0x0000000F;

				switch(l2desc & 3)
				{
				  case 0:
					return PAGE_TRANSLATION_FAULT;
  				  case 3:
					return PAGE_TRANSLATION_FAULT;
				case 1:
					entry.mapping = TLB_LARGEPAGE;
					break;
				case 2:
					entry.mapping = TLB_SMALLPAGE;
					break;
				}
			}
			break;
		  case 2:
			entry.virt_addr = virt_addr;
			entry.phys_addr = l1desc;
			entry.perms = l1desc & 0x00000C0C;
			entry.domain = (l1desc >> 5) & 0x0000000F;
			entry.mapping = TLB_SECTION;
			break;
		}
		entry.virt_addr &= tlb_masks[entry.mapping];
		entry.phys_addr &= tlb_masks[entry.mapping];

		/* place entry in the tlb */
		*tlb = &tlb_t->entrys[tlb_t->cycle];
		tlb_t->cycle = (tlb_t->cycle + 1) % tlb_t->num;
		**tlb = entry;
	}
	return NO_FAULT;
}



int checkPerms(int ap, int read)
{
	int s, r, user;

	s = ( CP15(1) & CONTROL_SYSTEM );
	r = ( CP15(1) & CONTROL_ROM );
	//chy 2006-02-15 , should consider system mode, don't conside 26bit mode
	user = (CURMODE == MODE_USR);

	switch(ap)
	{
	  case 0:
		return read && ((s && !user) || r);
	  case 1:
		return !user;
	  case 2:
		return read || !user;
	  case 3:
		return 1;
	}
	return 0;
}


mmufault_t checkAccess(INT32 virt_addr, tlb_entry_t * tlb, int read)
{
	int access;

	access = (CP15(3) >> (tlb->domain * 2)) & 3;
	if((access == 0) || (access == 2))
	{
		/* It's unclear from the documentation whether this
		   should always raise a section domain fault, or if
		   it should be a page domain fault in the case of an
		   L1 that describes a page table.  In the ARM710T
		   datasheets, "Figure 8-9: Sequence for checking faults"
		   seems to indicate the former, while "Table 8-4: Priority
		   encoding of fault status" gives a value for FS[3210] in
		   the event of a domain fault for a page.  Hmm. */
		return SECTION_DOMAIN_FAULT;
	}
	if(access == 1)
	{
		/* client access - check perms */
		int subpage, ap;

		switch (tlb->mapping)
		{
		  case TLB_TINYPAGE:
			subpage = 0;
			//printf("TLB_TINYPAGE virt_addr=0x%x  \n",virt_addr );
			break;
  		  case TLB_SMALLPAGE:
			subpage = (virt_addr >> 10) & 3;
			break;
		  case TLB_LARGEPAGE:
			subpage = (virt_addr >> 14) & 3;
			break;
		  case TLB_SECTION:
			subpage = 3;
			break;
		  default:
			PROGRAM_ERROR;
			subpage = 0;	/* cleans a warning */
		}
		ap = (tlb->perms >> (subpage * 2 + 4)) & 3;
		if(!checkPerms(ap, read))
		{
			if(tlb->mapping == TLB_SECTION)
			{
				return SECTION_PERMISSION_FAULT;
			}
			else
			{
				return SUBPAGE_PERMISSION_FAULT;
			}
		}
	}
	else {			/* access == 3 */
		/* manager access - don't check perms */
	}
	return NO_FAULT;
}

