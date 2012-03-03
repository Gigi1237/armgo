#ifndef S3C2410_H_
#define S3C2410_H_

#include "common.h"
#include "thread.h"
#include "device/uart/uart.h"
#include "device/gpio/gpio.h"
#include "device/watchdog/watchdog.h"
#include "device/sdram/sdram.h"

typedef struct
{
	//uint32 srcpnd;		/* Indicate the interrupt request status */
	//uint32 intmod;		/* Interrupt mode register */
	//uint32 intmsk;		/* Determine which interrupt source is masked */
	//uint32 priority;		/* IRQ priority control register */
	//uint32 intpnd;		/* Indicate the interrupt request status */
	//uint32 intoffset;		/* Indicate the IRQ interrupt request source */
	//uint32 subsrcpnd;		/* Indicate the interrupt request status */
	//uint32 intsubmsk;		/* Determin which interrupt source is masked */

	//uint32 eintmask;		/* Interrupt pending register for 20 external interrupts (EINT[23:4]) */
	//uint32 eintpend;		/* Interrupt mask register for 20 external interrupts (EINT[23:4]). */
	//struct s3c2410x_timer_io timer;	/* Timer */
	UART uart[3];                       /* uart */
	//struct s3c2410x_clkpower clkpower;	/* clock and power management */
	//s3c2410_memctl_t memctl;
	//s3c2410_wd_timer_t wd_timer;
	IOPORT gpio_ctl[0xc0]; /* GPIO control register */

	WATCHDOG watchdog;

	SDRAM sdram;
}S3C2410;



extern S3C2410 s3c2410;


#define IO_PORT_START 0x48000000
#define IO_PORT_END   0x5f000000


void initS3c2410();

int readDevicePort(INT32 address);
void writeDevicePort(INT32 address, INT32 data);
INT32 getPCLK();


#endif
