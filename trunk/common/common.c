#include <stdio.h>
#include <stdarg.h>
#include "common.h"

ARMGO armgo;


INT32 andMask[32] =
{
  0x00000001,
  0x00000003,
  0x00000007,
  0x0000000f,
  0x0000001f,
  0x0000003f,
  0x0000007f,
  0x000000ff,
  0x000001ff,
  0x000003ff,
  0x000007ff,
  0x00000fff,
  0x00001fff,
  0x00003fff,
  0x00007fff,
  0x0000ffff,
  0x0001ffff,
  0x0003ffff,
  0x0007ffff,
  0x000fffff,
  0x001fffff,
  0x003fffff,
  0x007fffff,
  0x00ffffff,
  0x01ffffff,
  0x03ffffff,
  0x07ffffff,
  0x0fffffff,
  0x1fffffff,
  0x3fffffff,
  0x7fffffff,
  0xffffffff
};



void INFOLOG(const char * format, ...)
{
  printf("(Info) In %s: line %d, function %s: ", __FILE__, __LINE__, __FUNCTION__);
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
  printf("\n");
}



/* s1 and s2 MUST be INT64! */
inline INT64 multiplyUnsigned32(INT64 s1, INT64 s2)
{
  INT64 result;
  result = s1 * s2;
  return result;
}

/* s1 and s2 MUST be SIGNINT64! */
inline SIGNINT64 multiplySigned32(SIGNINT64 s1, SIGNINT64 s2)
{
  SIGNINT64 result;
  result = s1 * s2;
  return result;
}


/* n(from bit(0)-bit(b) ) will extend sign to 32bits */
inline INT32 SignExtend(INT32 n, int b)
{
  if( n & ( 1 << b ) )
    return ( n | ( 0xffffffff << (b + 1) ) );
  else
    return ( n & ( ~( 0xffffffff << (b + 1) ) ) );
}
