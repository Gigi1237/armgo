#include <stdio.h>
#include "s3c2410.h"

S3C2410 s3c2410;


void initS3c2410()
{
  sdramgInit();
  uartInit();
}

int readDevicePort(INT32 address)
{
  //printf("read [0x%08x]\n", address);

  /* uart port */
  if( (address >= UART_CTL_BASE0) && (address < UART_CTL_BASE0 + UART_CTL_SIZE))
  {
    return(uartRead(address, PORT_BLOCK));
  }

  //printf("unknown read port 0x%08x\n", address);

  return 0;
}

void writeDevicePort(INT32 address, INT32 data)
{
  //printf("write 0x%08x ==> 0x%08x\n", data, address);

  /* uart port */
  if( (address >= UART_CTL_BASE0) && (address < UART_CTL_BASE0 + UART_CTL_SIZE))
  {
    uartWrite(address, data, PORT_BLOCK);
    return;
  }

  /* gpio port */
  if( (address >= GPIO_CTL_BASE) && (address < (GPIO_CTL_BASE + GPIO_CTL_SIZE)) )
  {
    gpioWrite(address, data, PORT_BLOCK);
    return;
  }

  /* watchdog port */
  if( (address >= WATCHDOG_CTL_BASE) && (address < (WATCHDOG_CTL_BASE + WATCHDOG_CTL_SIZE)) )
  {
    watchdogWrite(address, data, PORT_BLOCK);
    return;
  }

  /* sdram port */
  if( (address >= SDRAM_CTL_BASE) && (address < (SDRAM_CTL_BASE + SDRAM_CTL_SIZE)) )
  {
    sdramWrite(address, data, PORT_BLOCK);
    return;
  }

  printf("unknown port 0x%08x, value=0x%08x\n", address, data);
}



INT32 getPCLK()
{
	return 40*1000*1000;     //40MHz
}
