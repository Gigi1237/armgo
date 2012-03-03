


#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include "common.h"

typedef struct
{
  //INT32 port;
  INT32 value;
  pthread_mutex_t mutex;
}IOPORT;


void threadInitPort(IOPORT * port, INT32 value, int block);
void threadSetPortValue(IOPORT * port, INT32 value, int block);
INT32 threadGetPortValue(IOPORT * port, int block);
void threadPortBitClear(IOPORT * port, int bit, int block);
void threadPortBitSet(IOPORT * port, int bit, int block);


#define PORT_BLOCK 1
#define PORT_UNBLOCK 0

#endif
