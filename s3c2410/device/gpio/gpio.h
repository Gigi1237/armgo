#ifndef GPIO_H_
#define GPIO_H_

#include "common.h"


/********************************************
* GPIO Controller Registers
********************************************/
#define GPIO_CTL_BASE		(0x56000000)
#define GPIO_CTL_SIZE		(0xC0)

#define GPACON		GPIO_CTL_BASE+(0x0)
#define GPADAT		GPIO_CTL_BASE+(0x4)
#define GPBCON		GPIO_CTL_BASE+(0x10)
#define GPBDAT		GPIO_CTL_BASE+(0x14)
#define GPBUP		GPIO_CTL_BASE+(0x18)
#define GPCCON		GPIO_CTL_BASE+(0x20)
#define GPCDAT		GPIO_CTL_BASE+(0x24)
#define GPCUP		GPIO_CTL_BASE+(0x28)
#define GPDCON		GPIO_CTL_BASE+(0x30)
#define GPDDAT		GPIO_CTL_BASE+(0x34)
#define GPDUP		GPIO_CTL_BASE+(0x38)
#define GPECON		GPIO_CTL_BASE+(0x40)
#define GPEDAT		GPIO_CTL_BASE+(0x44)
#define GPEUP		GPIO_CTL_BASE+(0x48)
#define GPFCON		GPIO_CTL_BASE+(0x50)
#define GPFDAT		GPIO_CTL_BASE+(0x54)
#define GPFUP		GPIO_CTL_BASE+(0x58)
#define GPGCON		GPIO_CTL_BASE+(0x60)
#define GPGDAT		GPIO_CTL_BASE+(0x64)
#define GPGUP		GPIO_CTL_BASE+(0x68)
#define GPHCON		GPIO_CTL_BASE+(0x70)
#define GPHDAT		GPIO_CTL_BASE+(0x74)
#define GPHUP		GPIO_CTL_BASE+(0x78)
#define MISCCR		GPIO_CTL_BASE+(0x80)
#define DCLKCON		GPIO_CTL_BASE+(0x84)
#define EXTINT0		GPIO_CTL_BASE+(0x88)
#define EXTINT1		GPIO_CTL_BASE+(0x8c)
#define EXTINT2		GPIO_CTL_BASE+(0x90)
#define EINTFLT0	GPIO_CTL_BASE+(0x94)
#define EINTFLT1	GPIO_CTL_BASE+(0x98)
#define EINTFLT2	GPIO_CTL_BASE+(0x9c)
#define EINTFLT3	GPIO_CTL_BASE+(0xa0)
#define EINTMASK	GPIO_CTL_BASE+(0xa4)
#define EINTPEND	GPIO_CTL_BASE+(0xa8)
#define GSTATUS0	GPIO_CTL_BASE+(0xac)
#define GSTATUS1	GPIO_CTL_BASE+(0xb0)
#define GSTATUS2	GPIO_CTL_BASE+(0xb4)
#define GSTATUS3	GPIO_CTL_BASE+(0xb8)
#define GSTATUS4	GPIO_CTL_BASE+(0xbc)


void gpioWrite(INT32 address, INT32 data, int block);
INT32 gpioRead(INT32 address, int block);

#endif
