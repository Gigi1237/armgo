
#include <stdio.h>
#include "s3c2410.h"
#include "watchdog.h"


int watchdogInit()
{
  threadSetPortValue(&s3c2410.watchdog.wtcon, 0x8021, PORT_BLOCK);
  threadSetPortValue(&s3c2410.watchdog.wtdat, 0x8000, PORT_BLOCK);
  threadSetPortValue(&s3c2410.watchdog.wtcnt, 0x8000, PORT_BLOCK);

  return 0;
}

int watchdogWriteWTCON(INT32 data)
{
  threadSetPortValue(&s3c2410.watchdog.wtcnt, data, PORT_BLOCK);

  if(GETBIT(data, 0) == 1)
  {
	  printf("watchdog enabled\n");
  }
  else
  {
	  printf("watchdog disabled\n");
  }
	return 0;
}

void watchdogWrite(INT32 address, INT32 data, int block)
{
  switch(address)
  {
    case WTCON:
    	watchdogWriteWTCON(data);
    	break;
    case WTDAT:
    	  threadSetPortValue(&s3c2410.watchdog.wtdat, data, block);
    	break;
    case WTCNT:
    	  threadSetPortValue(&s3c2410.watchdog.wtcnt, data, block);
    	break;
    default:
    	printf("Unknown watchdog port\n");
    	break;
  }
}



int watchdogRead(INT32 address)
{
  return 0;
}

