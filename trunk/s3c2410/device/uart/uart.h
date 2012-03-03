#ifndef UART_H_
#define UART_H_

#include "common.h"
#include "thread.h"

typedef struct
{
	IOPORT ulcon;		/* UART line control register */
	IOPORT ucon;		/* UART control register */
	IOPORT ufcon;		/* UART FIFO control register */
	IOPORT umcon;		/* UART Modem control register */
	IOPORT utrstat;		/* UART Tx/Rx status register */
	IOPORT uerstat;		/* UART Rx error status register */
	IOPORT ufstat;		/* UART FIFO status register */
	IOPORT umstat;		/* UART Modem status register */
	IOPORT utxh;		/* UART transmit buffer register */
	IOPORT urxh;		/* UART receive buffer register */
	IOPORT ubrdiv;		/* Baud rate divisor register 0 */
}UART;



/********************************************
* UART Control Registers
********************************************/
#define UART_CTL_BASE0		(0x50000000)
#define UART_CTL_SIZE		(0x802c)

#define ULCON		(0x00)
#define UCON		(0x04)
#define UFCON		(0x08)
#define UMCON		(0x0c)
#define UTRSTAT		(0x10)
#define UERSTAT		(0x14)
#define UFSTAT		(0x18)
#define UMSTAT		(0x1c)
#define UTXH		(0x20)
#define URXH		(0x24)
#define UBRDIV		(0x28)


#define UART_INT_RXD		0x1
#define UART_INT_TXD		0x2
#define UART_INT_EXD		0x4

#define UART_UCON_INIT		0x5
#define UART_ULCON_INIT		0x3	//8N1
#define UART_UTRSTAT_INIT	0x6


void uartInit();
void uartWrite(INT32 address, INT32 data, int block);
INT32 uartRead(INT32 address, int block);


#endif
