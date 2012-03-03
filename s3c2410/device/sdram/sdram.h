/*
 * watchdog.h
 *
 *  Created on: 2012-2-28
 *      Author: hutao
 */

#ifndef SDRAM_H_
#define SDRAM_H_

#include "common.h"
#include "thread.h"


#define BWSCON   0x48000000
#define BANKCON0 0x48000004
#define BANKCON1 0x48000008
#define BANKCON2 0x4800000C
#define BANKCON3 0x48000010
#define BANKCON4 0x48000014
#define BANKCON5 0x48000018
#define BANKCON6 0x4800001C
#define BANKCON7 0x48000020
#define REFRESH  0x48000024
#define BANKSIZE 0x48000028
#define MRSRB6   0x4800002C
#define MRSRB7   0x48000030


#define SDRAM_CTL_BASE		(0x48000000)
#define SDRAM_CTL_SIZE		(0x34)


typedef struct
{
	IOPORT bwscon;
	IOPORT bankcon0;
	IOPORT bankcon1;
	IOPORT bankcon2;
	IOPORT bankcon3;
	IOPORT bankcon4;
	IOPORT bankcon5;
	IOPORT bankcon6;
	IOPORT bankcon7;
	IOPORT refresh;
	IOPORT banksize;
	IOPORT mrsrb6;
	IOPORT mrsrb7;

}SDRAM;



void sdramWrite(INT32 address, INT32 data, int block);


#endif /* SDRAM_H_ */
