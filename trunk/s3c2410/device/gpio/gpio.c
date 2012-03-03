#include "gpio.h"
#include "s3c2410.h"



void gpioWrite(INT32 address, INT32 data, int block)
{
  threadSetPortValue(&s3c2410.gpio_ctl[address - GPIO_CTL_BASE], data, block);
}
