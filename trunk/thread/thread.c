#include "thread.h"



void threadInitPort(IOPORT * port, INT32 value, int block)
{
  pthread_mutex_init(&port->mutex, NULL);
  threadSetPortValue(port, value, block);
}


void threadPortBitClear(IOPORT * port, int bit, int block)
{
  if(block == PORT_BLOCK)
  {
	pthread_mutex_lock(&port->mutex);
	port->value &= (~(1 << bit));
    pthread_mutex_unlock(&port->mutex);
  }
  else
  {
    pthread_mutex_trylock(&port->mutex);
	port->value &= (~(1 << bit));
	pthread_mutex_unlock(&port->mutex);
  }
}

void threadPortBitSet(IOPORT * port, int bit, int block)
{
  if(block == PORT_BLOCK)
  {
	pthread_mutex_lock(&port->mutex);
	port->value |= (1 << bit);
    pthread_mutex_unlock(&port->mutex);
  }
  else
  {
    pthread_mutex_trylock(&port->mutex);
	port->value |= (1 << bit);
	pthread_mutex_unlock(&port->mutex);
  }
}



void threadSetPortValue(IOPORT * port, INT32 value, int block)
{
  if(block == PORT_BLOCK)
  {
	pthread_mutex_lock(&port->mutex);
	port->value = value;
    pthread_mutex_unlock(&port->mutex);
  }
  else
  {
    pthread_mutex_trylock(&port->mutex);
	port->value = value;
	pthread_mutex_unlock(&port->mutex);
  }
}


INT32 threadGetPortValue(IOPORT * port, int block)
{
  INT32 data;

  if(block == PORT_BLOCK)
  {
	pthread_mutex_lock(&port->mutex);
	data = port->value;
    pthread_mutex_unlock(&port->mutex);
  }
  else
  {
    pthread_mutex_trylock(&port->mutex);
	data = port->value;
	pthread_mutex_unlock(&port->mutex);
  }

  return data;
}
