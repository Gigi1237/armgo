/*
 * watchdog.h
 *
 *  Created on: 2012-2-28
 *      Author: hutao
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "common.h"
#include "thread.h"


#define WTCON 0x53000000
#define WTDAT 0x53000004
#define WTCNT 0x53000008

#define WATCHDOG_CTL_BASE		(0x53000000)
#define WATCHDOG_CTL_SIZE		(0xc)


typedef struct
{
  IOPORT wtcon;
  IOPORT wtdat;
  IOPORT wtcnt;
}WATCHDOG;



void watchdogWrite(INT32 address, INT32 data, int block);


#endif /* WATCHDOG_H_ */
