
#include <stdio.h>
#include "s3c2410.h"
#include "sdram.h"


int sdramgInit()
{
  threadSetPortValue(&s3c2410.sdram.bwscon,   0x00000000, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon0, 0x00000700, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon1, 0x00000700, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon2, 0x00000700, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon3, 0x00000700, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon4, 0x00000700, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon5, 0x00000700, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon6, 0x00018008, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.bankcon7, 0x00018008, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.refresh,  0x00ac0000, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.banksize, 0x00000000, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.mrsrb6,   0x00000000, PORT_BLOCK);
  threadSetPortValue(&s3c2410.sdram.mrsrb7,   0x00000000, PORT_BLOCK);

  return 0;
}


void sdramWrite(INT32 address, INT32 data, int block)
{
  switch(address)
  {
    case BWSCON:
    	threadSetPortValue(&s3c2410.sdram.bwscon,   data, block);
    	break;
    case BANKCON0:
    	threadSetPortValue(&s3c2410.sdram.bankcon0, data, block);
    	break;
    case BANKCON1:
    	threadSetPortValue(&s3c2410.sdram.bankcon1, data, block);
    	break;
    case BANKCON2:
    	threadSetPortValue(&s3c2410.sdram.bankcon2, data, block);
    	break;
    case BANKCON3:
    	threadSetPortValue(&s3c2410.sdram.bankcon3, data, block);
    	break;
    case BANKCON4:
    	threadSetPortValue(&s3c2410.sdram.bankcon4, data, block);
    	break;
    case BANKCON5:
    	threadSetPortValue(&s3c2410.sdram.bankcon5, data, block);
    	break;
    case BANKCON6:
    	threadSetPortValue(&s3c2410.sdram.bankcon6, data, block);
    	break;
    case BANKCON7:
    	threadSetPortValue(&s3c2410.sdram.bankcon7, data, block);
    	break;
    case REFRESH:
    	threadSetPortValue(&s3c2410.sdram.refresh,  data, block);
    	break;
    case BANKSIZE:
    	threadSetPortValue(&s3c2410.sdram.banksize, data, block);
    	break;
    case MRSRB6:
    	threadSetPortValue(&s3c2410.sdram.mrsrb6,   data, block);
    	break;
    case MRSRB7:
    	threadSetPortValue(&s3c2410.sdram.mrsrb7,   data, block);
    	break;
    default:
    	printf("Unknown sdram port\n");
    	break;
  }
}



int sdramRead(INT32 address)
{
  return 0;
}

