#include <stdio.h>
#include "uart.h"
#include "s3c2410.h"
#include "window.h"



void uartInit()
{
  int i;
  for(i=0; i<3; ++i)
  {
    threadInitPort(&s3c2410.uart[i].ulcon,   0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].ucon,    0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].ufcon,   0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].umcon,   0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].utrstat, 0x06, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].uerstat, 0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].ufstat,  0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].umstat,  0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].utxh,    0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].urxh,    0x00, PORT_BLOCK);
    threadInitPort(&s3c2410.uart[i].ubrdiv,  0x00, PORT_BLOCK);
  }
}



INT32 getUartClock(int index)
{
  INT32 data =  threadGetPortValue(&s3c2410.uart[index].ucon, PORT_BLOCK);
  if(GETBIT(data, 10) == 0)
  {
	return getPCLK();
  }
  else
  {
	return 40*1000*1000;  //UEXTCLK
  }
}


INT32 uartRead(INT32 address, int block)
{
//	printf("read [0x%08x]\n", address);
  int index, offset;

  address -= UART_CTL_BASE0;
  index  = ( address / 0x4000 );
  offset = ( address % 0x4000 );

  switch(offset)
  {
	case ULCON:
      return threadGetPortValue(&s3c2410.uart[index].ulcon, block);
	case UCON:
	  return threadGetPortValue(&s3c2410.uart[index].ucon, block);
    case UFCON:
      return threadGetPortValue(&s3c2410.uart[index].ufcon, block);
	case UMCON:
	  return threadGetPortValue(&s3c2410.uart[index].umcon, block);
	case UTRSTAT:
	  return threadGetPortValue(&s3c2410.uart[index].utrstat, block);
	case UERSTAT:
	  return threadGetPortValue(&s3c2410.uart[index].uerstat, block);
	case UFSTAT:
	  return threadGetPortValue(&s3c2410.uart[index].ufstat, block);
	case UMSTAT:
	  return threadGetPortValue(&s3c2410.uart[index].umstat, block);
	case UTXH:
      return threadGetPortValue(&s3c2410.uart[index].utxh, block);
	case URXH:
	  threadPortBitClear(&s3c2410.uart[index].utrstat, 0, PORT_BLOCK);
      return threadGetPortValue(&s3c2410.uart[index].urxh, block);
    case UBRDIV:
  	  return threadGetPortValue(&s3c2410.uart[index].ubrdiv, block);
	default:
	  printf("Unknown uart read port!\n");
	  return 0xffffffff;
  }
}


void uartWrite(INT32 address, INT32 data, int block)
{
//	printf("write 0x%08x to [0x%08x]\n", data, address);

  int index, offset;

  address -= UART_CTL_BASE0;
  index  = ( address / 0x4000 );
  offset = ( address % 0x4000 );

  switch(offset)
  {
	case ULCON:
	  threadSetPortValue(&s3c2410.uart[index].ulcon, data, block);
      break;
	case UCON:
      threadSetPortValue(&s3c2410.uart[index].ucon, data, block);
	  break;
    case UFCON:
  	  threadSetPortValue(&s3c2410.uart[index].ufcon, data, block);
	  break;
	case UMCON:
	  threadSetPortValue(&s3c2410.uart[index].umcon, data, block);
	  break;
	case UTRSTAT:
	  threadSetPortValue(&s3c2410.uart[index].utrstat, data, block);
	  break;
	case UERSTAT:
	  threadSetPortValue(&s3c2410.uart[index].uerstat, data, block);
	  break;
	case UFSTAT:
	  threadSetPortValue(&s3c2410.uart[index].ufstat, data, block);
	  break;
	case UMSTAT:
	  threadSetPortValue(&s3c2410.uart[index].umstat, data, block);
	  break;
	case UTXH:
	  threadSetPortValue(&s3c2410.uart[index].utxh, data, block);
	  threadPortBitClear(&s3c2410.uart[index].utrstat, 2, PORT_BLOCK);
	  window.charBuffer[window.count++] = (char)data;
	  window.refresh = 1;
	  printf("%c", data);
	  break;
	case URXH:
	  threadSetPortValue(&s3c2410.uart[index].urxh, data, block);
	  break;
    case UBRDIV:
      threadSetPortValue(&s3c2410.uart[index].ubrdiv, data, block);
	  break;
	default:
	  printf("Unknown uart write port!\n");
	  break;
  }
}
