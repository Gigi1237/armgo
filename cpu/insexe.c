#include <stdio.h>
#include <math.h>
#include "cpu.h"
#include "insdef.h"
#include "insexe.h"


SIGNINT64 SignSaturationTable[33][2];




void initSignSaturationTable()
{
  int i;

  /* the first is not used */
  SignSaturationTable[0][0] = SignSaturationTable[0][1] = 0;

  for(i=1; i<=32; ++i)
  {
	SignSaturationTable[i][0] = -(INT64)pow(2, i-1);
	SignSaturationTable[i][1] = (INT64)pow(2, i-1) - 1;
  }
}

inline int SignedDoesSat_add(INT32 a, INT32 b, int n)
{
  SIGNINT64 s1 = (SIGNINT64)(SIGNINT32)a;
  SIGNINT64 s2 = (SIGNINT64)(SIGNINT32)b;
  SIGNINT64 x = s1 + s2;

  if( ( x >= SignSaturationTable[n][0] ) && ( x <= SignSaturationTable[n][1] ) )
  {
	return 0;
  }
  else
  {
	return 1;
  }
}

inline int SignedDoesSat_sub(INT32 a, INT32 b, int n)
{
  SIGNINT64 s1 = (SIGNINT64)(SIGNINT32)a;
  SIGNINT64 s2 = (SIGNINT64)(SIGNINT32)b;
  SIGNINT64 x = s1 - s2;

  if( ( x >= SignSaturationTable[n][0] ) && ( x <= SignSaturationTable[n][1] ) )
  {
	return 0;
  }
  else
  {
	return 1;
  }
}


inline INT32 SignedSat_add(INT32 a, INT32 b, int n)
{
  SIGNINT64 s1 = (SIGNINT64)(SIGNINT32)a;
  SIGNINT64 s2 = (SIGNINT64)(SIGNINT32)b;
  SIGNINT64 x = s1 + s2;

  if( x < SignSaturationTable[n][0] )
  {
	return (INT32)SignSaturationTable[n][0];
  }
  else if( x >SignSaturationTable[n][1] )
  {
	return (INT32)SignSaturationTable[n][1];
  }
  else
  {
    return (INT32)x;
  }
}


inline INT32 SignedSat_sub(INT32 a, INT32 b, int n)
{
  SIGNINT64 s1 = (SIGNINT64)(SIGNINT32)a;
  SIGNINT64 s2 = (SIGNINT64)(SIGNINT32)b;
  SIGNINT64 x = s1 - s2;

  if( x < SignSaturationTable[n][0] )
  {
	return (INT32)SignSaturationTable[n][0];
  }
  else if( x >SignSaturationTable[n][1] )
  {
	return (INT32)SignSaturationTable[n][1];
  }
  else
  {
    return (INT32)x;
  }
}




void initInstruction()
{
  initSignSaturationTable();
}


inline int CurrentModeHasSPSR()
{
  if( (CURMODE == MODE_USR) || (CURMODE == MODE_SYS) )
	return 0;
  else
	return 1;
}


/* only for addressing mode 4 use */
inline int Number_Of_Set_Bits_In(INT32 r)
{
  int i;
  int number;

  for(i=0, number=0; i<16; ++i)
    number += ((r >> i) & 1);

  return number;
}


// not implement!
inline int high_vectors_configured()
{
  return 0;
}







/********************************************************
 *
 * return whether an addition of a and b overflowed.
 * a:  0      a:  1
 * b:  0  or  b:  1
 * r:  1      r:  0
 *
 *******************************************************/
inline int OverflowFrom_add(INT32 a, INT32 b)
{
  INT32 r = a + b;

  return ( ( NEG(a) && NEG(b) && POS(r) ) || ( POS(a) && POS(b) && NEG(r) ) );
}

/********************************************************
 *
 * return whether an subtraction of a and b overflowed.
 * a:  0      a:  1
 * b:  1  or  b:  0
 * r:  1      r:  0
 *
 *******************************************************/
inline int OverflowFrom_sub(INT32 a, INT32 b)
{
  INT32 r = a - b;

  return ( ( NEG(a) && POS(b) && POS(r) ) || ( POS(a) && NEG(b) && NEG(r) ) );
}

/********************************************************
 *
 * return whether an addition of a and b carry out
 * a:  1      a:  1      b:  1
 * b:  1  or  r:  0  or  r:  0
 *
 *******************************************************/
inline int CarryFrom(INT32 a, INT32 b)
{
  INT32 r = a + b;

  return ( (NEG(a) && NEG(b)) || (NEG(a) && POS(r)) || (NEG(b) && POS(r)) );
}


/********************************************************
 *
 * return whether an subtraction of a and b carry out
 * a:  1      a:  1      b:  0
 * b:  0  or  r:  0  or  r:  0
 *
 *******************************************************/
inline int BorrowFrom(INT32 a, INT32 b)
{
  INT32 r = a - b;

  //return ( (NEG(a) && POS(b)) || (NEG(a) && POS(r)) || (POS(b) && POS(r)));
  //return (CarryFrom(r, b));
  return ( a < b );
}





/********************************************************
 *
 * return whether an addition of a, b and cflag carry out.
 *
 *******************************************************/
inline int CarryFromCflag(INT32 a, INT32 b, INT32 cf)
{
  if(cf)
  {
	if(CarryFrom(a, b))
      return 1;

	if(a + b == 0xffffffff)
	  return 1;

	return 0;
  }
  else
  {
	return CarryFrom(a, b);
  }
}



/********************************************************
 *
 * return whether an subtraction of a, b and cflag carry out.
 *
 *******************************************************/
inline int BorrowFromCflag(INT32 a, INT32 b, INT32 cf)
{
  if(cf)
  {
	if(BorrowFrom(a, b))
      return 1;

	if(a == b)
	  return 1;

	return 0;
  }
  else
  {
	return BorrowFrom(a, b);
  }
}



/********************************************************
 *
 * return whether an addition of a, b and cflag overflowed
 *
 *******************************************************/
inline int OverflowFromCflag_add(INT32 a, INT32 b, INT32 cf)
{
  if(cf)
  {
	if(OverflowFrom_add(a, b))
      return 1;

	return OverflowFrom_add(a + b, cf);
  }
  else
  {
	return OverflowFrom_add(a, b);
  }
}

/********************************************************
 *
 * return whether an subtraction of a, b and cflag overflowed
 *
 *******************************************************/
inline int OverflowFromCflag_sub(INT32 a, INT32 b, INT32 cf)
{
  if(cf)
  {
	if(OverflowFrom_sub(a, b))
      return 1;

	return OverflowFrom_sub(a - b, cf);
  }
  else
  {
	return OverflowFrom_sub(a, b);
  }
}





/***************************************
 *
 * Addressing mode 1
 * Data-processing operands
 *
 ***************************************/
#define DATA_PROCESSING_OPERANDS                                 \
{                                                                \
  n = BITS(19, 16);                                              \
  d = BITS(15, 12);                                              \
  s = BITS(11,  8);                                              \
  m = BITS( 3,  0);                                              \
  rn = Reg(n);                                                   \
  rd = Reg(d);                                                   \
  rs = Reg(s);                                                   \
  rm = Reg(m);                                                   \
  rotate_imm = BITS(11, 8);                                      \
  shift_imm  = BITS(11, 7);                                      \
  shift      = BITS( 6, 5);                                      \
                                                                 \
  if(BITS(27, 25) == 1)                                          \
  {                                                              \
    shifter_operand = ROR32(BITS(7, 0), rotate_imm * 2);         \
    if(rotate_imm == 0)                                          \
    {                                                            \
      shifter_carry_out = CFLAG;                                 \
    }                                                            \
    else                                                         \
    {                                                            \
      shifter_carry_out = GETBIT(shifter_operand, 31);         \
    }                                                            \
  }                                                              \
  else if(BITS(27, 25) == 0)                                     \
  {                                                              \
    switch(BITS(6, 4))                                           \
    {                                                            \
      case _000_:                                                \
        if(shift_imm == 0)                                       \
        {                                                        \
          shifter_operand = rm;                                  \
          shifter_carry_out = CFLAG;                             \
        }                                                        \
        else                                                     \
        {                                                        \
          shifter_operand = LSL32(rm, shift_imm);                \
          shifter_carry_out = GETBIT(rm, 32 - shift_imm);      \
        }                                                        \
        break;                                                   \
      case _001_:                                                \
        {                                                        \
          INT32 rs_t = GETBITS(rs, 7, 0);                      \
          if(rs_t == 0)                                          \
          {                                                      \
            shifter_operand = rm;                                \
            shifter_carry_out = CFLAG;                           \
          }                                                      \
          else if(rs_t < 32)                                     \
          {                                                      \
            shifter_operand = LSL32(rm, rs_t);                   \
            shifter_carry_out = GETBIT(rm, 32 - rs_t);         \
          }                                                      \
          else if(rs_t == 32)                                    \
          {                                                      \
            shifter_operand = 0;                                 \
            shifter_carry_out = GETBIT(rm, 0);                 \
          }                                                      \
          else                                                   \
          {                                                      \
            shifter_operand = 0;                                 \
            shifter_carry_out = 0;                               \
          }                                                      \
        }                                                        \
        break;                                                   \
      case _010_:                                                \
        if(shift_imm == 0)                                       \
        {                                                        \
          shifter_operand = 0;                                   \
          shifter_carry_out = GETBIT(rm, 31);                  \
        }                                                        \
        else                                                     \
        {                                                        \
          shifter_operand = LSR32(rm, shift_imm);                \
          shifter_carry_out = GETBIT(rm, shift_imm - 1);       \
        }                                                        \
        break;                                                   \
      case _011_:                                                \
        {                                                        \
          INT32 rs_t = GETBITS(rs, 7, 0);                      \
          if(rs_t == 0)                                          \
          {                                                      \
            shifter_operand = rm;                                \
            shifter_carry_out = CFLAG;                           \
          }                                                      \
          else if(rs_t < 32)                                     \
          {                                                      \
            shifter_operand = LSR32(rm, rs_t);                   \
            shifter_carry_out = GETBIT(rm, rs_t - 1);          \
          }                                                      \
          else if(rs_t == 32)                                    \
          {                                                      \
            shifter_operand = 0;                                 \
            shifter_carry_out = GETBIT(rm, 31);                \
          }                                                      \
          else                                                   \
          {                                                      \
            shifter_operand = 0;                                 \
            shifter_carry_out = 0;                               \
          }                                                      \
        }                                                        \
        break;                                                   \
      case _100_:                                                \
        if(shift_imm == 0)                                       \
        {                                                        \
          if(GETBIT(rm, 31) == 0)                              \
          {                                                      \
            shifter_operand = 0;                                 \
            shifter_carry_out = GETBIT(rm, 31);                \
          }                                                      \
          else                                                   \
          {                                                      \
            shifter_operand = 0xffffffff;                        \
            shifter_carry_out = GETBIT(rm, 31);                \
          }                                                      \
        }                                                        \
        else                                                     \
        {                                                        \
          shifter_operand = ASR32(rm, shift_imm);                \
          shifter_carry_out = GETBIT(rm, shift_imm - 1);       \
        }                                                        \
        break;                                                   \
      case _101_:                                                \
        {                                                        \
          INT32 rs_t = GETBITS(rs, 7, 0);                      \
          if(rs_t == 0)                                          \
          {                                                      \
            shifter_operand = rm;                                \
            shifter_carry_out = CFLAG;                           \
          }                                                      \
          else if(rs_t < 32)                                     \
          {                                                      \
            shifter_operand = ASR32(rm, rs_t);                   \
            shifter_carry_out = GETBIT(rm, rs_t - 1);          \
          }                                                      \
          else                                                   \
          {                                                      \
            if(GETBIT(rm, 31) == 0)                            \
            {                                                    \
              shifter_operand = 0;                               \
              shifter_carry_out = GETBIT(rm, 31);              \
            }                                                    \
            else                                                 \
            {                                                    \
              shifter_operand = 0xffffffff;                      \
              shifter_carry_out = GETBIT(rm, 31);              \
            }                                                    \
          }                                                      \
        }                                                        \
        break;                                                   \
      case _110_:                                                \
        if(shift_imm == 0)                                       \
        {                                                        \
          shifter_operand = LSL32(CFLAG, 31) | LSR32(rm, 1);     \
          shifter_carry_out = GETBIT(rm, 0);                   \
        }                                                        \
        else                                                     \
        {                                                        \
          shifter_operand = ROR32(rm, shift_imm);                \
          shifter_carry_out = GETBIT(rm, shift_imm - 1);       \
        }                                                        \
        break;                                                   \
      case _111_:                                                \
        {                                                        \
          INT32 rs_t = GETBITS(rs, 7, 0);                      \
          if(rs_t == 0)                                          \
          {                                                      \
            shifter_operand = rm;                                \
            shifter_carry_out = CFLAG;                           \
          }                                                      \
          else                                                   \
          {                                                      \
            rs_t = GETBITS(rs, 4, 0);                          \
            if(rs_t == 0)                                        \
            {                                                    \
              shifter_operand = rm;                              \
              shifter_carry_out = GETBIT(rm, 31);              \
            }                                                    \
            else                                                 \
            {                                                    \
              shifter_operand = ROR32(rm, rs_t);                 \
              shifter_carry_out = GETBIT(rm, rs_t - 1);        \
            }                                                    \
          }                                                      \
        }                                                        \
        break;                                                   \
      default:                                                   \
        PROGRAM_ERROR;                                           \
        break;                                                   \
    }                                                            \
  }                                                              \
}



/***************************************
 *
 * Addressing mode 2
 * Load and Store Word or Unsigned Byte
 *
 ***************************************/
#define LOAD_STORE_WORD_BYTE                                          \
{                                                                     \
  int ubit = BIT(23);                                                 \
  rn = Reg(BITS(19, 16));                                             \
  rd = Reg(BITS(15, 12));                                             \
  rm = Reg(BITS( 3,  0));                                             \
  offset_12 = BITS(11, 0);                                            \
  shift_imm = BITS(11, 7);                                            \
  shift     = BITS(6, 5);                                             \
                                                                      \
  switch(BITS(27, 20) & 0x32)                                         \
  {                                                                   \
    case ADDRESSING_MODE2_IMMOFF:                                     \
      if(ubit == 1)                                                   \
      {                                                               \
        address = rn + offset_12;                                     \
      }                                                               \
      else                                                            \
      {                                                               \
        address = rn - offset_12;                                     \
      }                                                               \
      break;                                                          \
    case ADDRESSING_MODE2_SCALEDREGOFF:                               \
      {                                                               \
        INT32 index;                                                  \
        switch(shift)                                                 \
        {                                                             \
          /* LSL */                                                   \
          case _00_:                                                  \
            index = LSL32(rm, shift_imm);                             \
            break;                                                    \
          /* LSR */                                                   \
          case _01_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              index = 0;                                              \
            }                                                         \
            else                                                      \
            {                                                         \
              index = LSR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          /* ASR */                                                   \
          case _10_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              if(GETBIT(rm, 31) == 1)                               \
              {                                                       \
                index = 0xffffffff;                                   \
              }                                                       \
              else                                                    \
              {                                                       \
                index = 0;                                            \
              }                                                       \
            }                                                         \
            else                                                      \
            {                                                         \
              index = ASR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          /* ROR or RRX */                                            \
          case _11_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              index = ( LSL32(CFLAG, 31) | LSR32(rm, 1) );            \
            }                                                         \
            else                                                      \
            {                                                         \
              index = ROR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          default:                                                    \
            PROGRAM_ERROR;                                            \
            break;                                                    \
        }                                                             \
        if(ubit == 1)                                                 \
        {                                                             \
          address = rn + index;                                       \
        }                                                             \
        else                                                          \
        {                                                             \
          address = rn - index;                                       \
        }                                                             \
      }                                                               \
      break;                                                          \
    case ADDRESSING_MODE2_IMMPREINDEX:                                \
      if(ubit == 1)                                                   \
      {                                                               \
        address = rn + offset_12;                                     \
      }                                                               \
      else                                                            \
      {                                                               \
        address = rn - offset_12;                                     \
      }                                                               \
      rn = address;                                                   \
      writeReg(BITS(19, 16), rn);                                     \
      break;                                                          \
    case ADDRESSING_MODE2_SCALEDREGPREINDEX:                          \
      {                                                               \
        INT32 index;                                                  \
        switch(shift)                                                 \
        {                                                             \
          /* LSL */                                                   \
          case _00_:                                                  \
            index = LSL32(rm, shift_imm);                             \
            break;                                                    \
          /* LSR */                                                   \
          case _01_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              index = 0;                                              \
            }                                                         \
            else                                                      \
            {                                                         \
              index = LSR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          /* ASR */                                                   \
          case _10_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              if(GETBIT(rm, 31) == 1)                               \
              {                                                       \
                index = 0xffffffff;                                   \
              }                                                       \
              else                                                    \
              {                                                       \
                index = 0;                                            \
              }                                                       \
            }                                                         \
            else                                                      \
            {                                                         \
              index = ASR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          /* ROR or RRX */                                            \
          case _11_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              index = ( LSL32(CFLAG, 31) | LSR32(rm, 1) );            \
            }                                                         \
            else                                                      \
            {                                                         \
              index = ROR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          default:                                                    \
            PROGRAM_ERROR;                                            \
            break;                                                    \
        }                                                             \
        if(ubit == 1)                                                 \
        {                                                             \
          address = rn + index;                                       \
        }                                                             \
        else                                                          \
        {                                                             \
          address = rn - index;                                       \
        }                                                             \
        rn = address;                                                 \
        writeReg(BITS(19, 16), rn);                                   \
      }                                                               \
      break;                                                          \
    case ADDRESSING_MODE2_IMMPOSTINDEX:                               \
      address = rn;                                                   \
      if(ubit == 1)                                                   \
      {                                                               \
        rn += offset_12;                                              \
      }                                                               \
      else                                                            \
      {                                                               \
        rn -= offset_12;                                              \
      }                                                               \
      writeReg(BITS(19, 16), rn);                                     \
      break;                                                          \
    case ADDRESSING_MODE2_SCALEDREGPOSTINDEX:                         \
      {                                                               \
    	address = rn;                                                 \
        INT32 index;                                                  \
        switch(shift)                                                 \
        {                                                             \
          /* LSL */                                                   \
          case _00_:                                                  \
            index = LSL32(rm, shift_imm);                             \
            break;                                                    \
          /* LSR */                                                   \
          case _01_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              index = 0;                                              \
            }                                                         \
            else                                                      \
            {                                                         \
              index = LSR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          /* ASR */                                                   \
          case _10_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              if(GETBIT(rm, 31) == 1)                               \
              {                                                       \
                index = 0xffffffff;                                   \
              }                                                       \
              else                                                    \
              {                                                       \
                index = 0;                                            \
              }                                                       \
            }                                                         \
            else                                                      \
            {                                                         \
              index = ASR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          /* ROR or RRX */                                            \
          case _11_:                                                  \
            if(shift_imm == 0)                                        \
            {                                                         \
              index = ( LSL32(CFLAG, 31) | LSR32(rm, 1) );            \
            }                                                         \
            else                                                      \
            {                                                         \
              index = ROR32(rm, shift_imm);                           \
            }                                                         \
            break;                                                    \
          default:                                                    \
            PROGRAM_ERROR;                                            \
            break;                                                    \
        }                                                             \
        if(ubit == 1)                                                 \
        {                                                             \
          rn += index;                                                \
        }                                                             \
        else                                                          \
        {                                                             \
          rn -= index;                                                \
        }                                                             \
        writeReg(BITS(19, 16), rn);                                   \
      }                                                               \
      break;                                                          \
    default:                                                          \
      PROGRAM_ERROR;                                                  \
      break;                                                          \
  }                                                                   \
}



/***************************************
 *
 * Addressing mode 3
 * Miscellaneous Loads and Stores
 *
 ***************************************/
#define LOAD_STORE_MISCELLANEOUS                                          \
{                                                                         \
  int ubit = BIT(23);                                                     \
  INT32 rn = Reg(BITS(19, 16));                                           \
  INT32 rd = Reg(BITS(15, 12));                                           \
  INT32 rm = Reg(BITS( 3,  0));                                           \
  INT32 immedH = BITS(11, 8);                                             \
  INT32 immedL = BITS( 3, 0);                                             \
                                                                          \
  switch(BITS(27, 20) & 0x16)                                             \
  {                                                                       \
    case ADDRESSING_MODE3_IMMOFF:                                         \
      {                                                                   \
        INT8 offset_8 = (INT8)( (immedH << 4) | immedL );                 \
        if(ubit == 1)                                                     \
        {                                                                 \
          address = rn + offset_8;                                        \
        }                                                                 \
        else                                                              \
        {                                                                 \
          address = rn - offset_8;                                        \
        }                                                                 \
      }                                                                   \
      break;                                                              \
    case ADDRESSING_MODE3_REGOFF:                                         \
      if(ubit == 1)                                                       \
      {                                                                   \
        address = rn + rm;                                                \
      }                                                                   \
      else                                                                \
      {                                                                   \
        address = rn - rm;                                                \
      }                                                                   \
      break;                                                              \
    case ADDRESSING_MODE3_IMMPREINDEX:                                    \
      {                                                                   \
        INT8 offset_8 = (INT8)( (immedH << 4) | immedL );                 \
        if(ubit == 1)                                                     \
        {                                                                 \
          address = rn + offset_8;                                        \
        }                                                                 \
        else                                                              \
        {                                                                 \
          address = rn - offset_8;                                        \
        }                                                                 \
        rn = address;                                                     \
        writeReg(BITS(19, 16), rn);                                       \
      }                                                                   \
      break;                                                              \
    case ADDRESSING_MODE3_REGPREINDEX:                                    \
      if(ubit == 1)                                                       \
      {                                                                   \
        address = rn + rm;                                                \
      }                                                                   \
      else                                                                \
      {                                                                   \
        address = rn - rm;                                                \
      }                                                                   \
      rn = address;                                                       \
      writeReg(BITS(19, 16), rn);                                         \
      break;                                                              \
    case ADDRESSING_MODE3_IMMPOSTINDEX:                                   \
      {                                                                   \
        address = rn;                                                     \
        INT8 offset_8 = (INT8)( (immedH << 4) | immedL );                 \
        if(ubit == 1)                                                     \
        {                                                                 \
          rn += offset_8;                                                 \
        }                                                                 \
        else                                                              \
        {                                                                 \
          rn -= offset_8;                                                 \
        }                                                                 \
        writeReg(BITS(19, 16), rn);                                       \
      }                                                                   \
      break;                                                              \
    case ADDRESSING_MODE3_REGPOSTINDEX:                                   \
      address = rn;                                                       \
      if(ubit == 1)                                                       \
      {                                                                   \
        rn += rm;                                                         \
      }                                                                   \
      else                                                                \
      {                                                                   \
        rn -= rm;                                                         \
      }                                                                   \
      writeReg(BITS(19, 16), rn);                                         \
      break;                                                              \
    default:                                                              \
      PROGRAM_ERROR;                                                      \
      break;                                                              \
  }                                                                       \
}




/***************************************
 *
 * Addressing mode 4
 * Load and Store Multiple
 *
 ***************************************/
#define LOAD_STORE_MULTIPLE                                              \
{                                                                        \
  rn = Reg(BITS(19, 16));                                                \
  register_list = BITS(15, 0);                                           \
  INT32 temp = Number_Of_Set_Bits_In(register_list) * 4;                 \
  int wbit = BIT(21);                                                    \
                                                                         \
  switch(BITS(24, 23))                                                   \
  {                                                                      \
    case ADDRESSING_MODE4_LSM_IA:                                        \
      start_address = rn;                                                \
      end_address = rn + temp - 4;                                       \
      if(wbit == 1)                                                      \
      {                                                                  \
        rn += temp;                                                      \
        writeReg(BITS(19, 16), rn);                                      \
      }                                                                  \
      break;                                                             \
    case ADDRESSING_MODE4_LSM_IB:                                        \
      start_address = rn + 4;                                            \
      end_address = rn + temp;                                           \
      if(wbit == 1)                                                      \
      {                                                                  \
        rn += temp;                                                      \
        writeReg(BITS(19, 16), rn);                                      \
      }                                                                  \
      break;                                                             \
    case ADDRESSING_MODE4_LSM_DA:                                        \
      start_address = rn - temp + 4;                                     \
      end_address = rn;                                                  \
      if(wbit == 1)                                                      \
      {                                                                  \
        rn -= temp;                                                      \
        writeReg(BITS(19, 16), rn);                                      \
      }                                                                  \
      break;                                                             \
    case ADDRESSING_MODE4_LSM_DB:                                        \
      start_address = rn - temp;                                         \
      end_address = rn - 4;                                              \
      if(wbit == 1)                                                      \
      {                                                                  \
        rn -= temp;                                                      \
        writeReg(BITS(19, 16), rn);                                      \
      }                                                                  \
      break;                                                             \
    default:                                                             \
      PROGRAM_ERROR;                                                     \
      break;                                                             \
  }                                                                      \
}





/***************************************
 *
 * Addressing mode 5
 * Load and Store Coprocessor
 *
 ***************************************/
#define LOAD_STORE_COPROCESSOR                                          \
{                                                                       \
  printf("Not implemented!\n");                                         \
}






int execIns(INT32 start)
{
  INT32 flag;

  /* Addressing mode 1 */
  INT32 n, d, s, m, rn, rd, rs, rm, shift_imm, shift, rotate_imm;
  INT32 shifter_operand;
  INT32 shifter_carry_out;

  /* Addressing mode 2 */
  INT32 address, offset_12;

  /* Addressing mode 4 */
  INT32 start_address, end_address, register_list;

    switch(TOPBITS(28))
    {
      case AL:
      case NV:
        flag = 1;
        break;
      case EQ:
        flag = ZFLAG;
        break;
      case NE:
        flag = !ZFLAG;
        break;
      case VS:
        flag = VFLAG;
        break;
      case VC:
        flag = !VFLAG;
        break;
      case MI:
        flag = NFLAG;
        break;
      case PL:
        flag = !NFLAG;
        break;
      case CS:
        flag = CFLAG;
        break;
      case CC:
        flag = !CFLAG;
        break;
      case HI:
        flag = ( CFLAG && !ZFLAG );
        break;
      case LS:
        flag = ( !CFLAG || ZFLAG );
        break;
      case GE:
        flag = ( (!NFLAG && !VFLAG) || (NFLAG && VFLAG) );
        break;
      case LT:
        flag = ( (NFLAG && !VFLAG) || (!NFLAG && VFLAG) );
        break;
      case GT:
        flag = ( (!NFLAG && !VFLAG && !ZFLAG) || (NFLAG && VFLAG && !ZFLAG) );
        break;
      case LE:
        flag = ( (NFLAG && !VFLAG) || (!NFLAG && VFLAG) || ZFLAG );
        break;
      default:
        PROGRAM_ERROR;
        break;
    }

    if(flag)
    {
      switch(BITS(27, 20))
      {
        case _00000000_:
          EXE_AND;
          EXE_LDRD;
          EXE_MUL;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00000001_:
          EXE_AND;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_MUL;
          INSTNOTSUPPORT;
          break;

        case _00000010_:
          EXE_EOR;
          EXE_LDRD;
          EXE_MLA;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00000011_:
          EXE_EOR;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_MLA;
          INSTNOTSUPPORT;
          break;

        case _00000100_:
          EXE_LDRD;
          EXE_STRD;
          EXE_STRH;
          EXE_SUB;
          EXE_UMAAL;
          INSTNOTSUPPORT;
          break;

        case _00000101_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_SUB;
          INSTNOTSUPPORT;
          break;

        case _00000110_:
          EXE_LDRD;
          EXE_RSB;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00000111_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_RSB;
          INSTNOTSUPPORT;
          break;

        case _00001000_:
          EXE_ADD;
          EXE_LDRD;
          EXE_STRD;
          EXE_STRH;
          EXE_UMULL;
          INSTNOTSUPPORT;
          break;

        case _00001001_:
          EXE_ADD;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_UMULL;
          INSTNOTSUPPORT;
          break;

        case _00001010_:
          EXE_ADC;
          EXE_LDRD;
          EXE_STRD;
          EXE_STRH;
          EXE_UMLAL;
          INSTNOTSUPPORT;
          break;

        case _00001011_:
          EXE_ADC;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_UMLAL;
          INSTNOTSUPPORT;
          break;

        case _00001100_:
          EXE_LDRD;
          EXE_SBC;
          EXE_SMULL;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00001101_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_SBC;
          EXE_SMULL;
          INSTNOTSUPPORT;
          break;

        case _00001110_:
          EXE_LDRD;
          EXE_RSC;
          EXE_SMLAL;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00001111_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_RSC;
          EXE_SMLAL;
          INSTNOTSUPPORT;
          break;

        case _00010000_:
          EXE_CPS;
          EXE_LDRD;
          EXE_MRS;
          EXE_QADD;
          EXE_SETEND;
          EXE_SMLAXY;
          EXE_STRD;
          EXE_STRH;
          EXE_SWP;
          INSTNOTSUPPORT;
          break;

        case _00010001_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_TST;
          INSTNOTSUPPORT;
          break;

        case _00010010_:
          EXE_BKPT;
          EXE_BLX2;
          EXE_BX;
          EXE_BXJ;
          EXE_LDRD;
          EXE_MSR;
          EXE_QSUB;
          EXE_SMLAWY;
          EXE_SMULWY;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00010011_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_TEQ;
          INSTNOTSUPPORT;
          break;

        case _00010100_:
          EXE_LDRD;
          EXE_MRS;
          EXE_QDADD;
          EXE_SMLALXY;
          EXE_STRD;
          EXE_STRH;
          EXE_SWPB;
          INSTNOTSUPPORT;
          break;

        case _00010101_:
          EXE_CMP;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          INSTNOTSUPPORT;
          break;

        case _00010110_:
          EXE_CLZ;
          EXE_LDRD;
          EXE_MSR;
          EXE_QDSUB;
          EXE_SMULXY;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00010111_:
          EXE_CMN;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          INSTNOTSUPPORT;
          break;

        case _00011000_:
          EXE_LDRD;
          EXE_ORR;
          EXE_STRD;
          EXE_STREX;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00011001_:
          EXE_LDREX;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_ORR;
          INSTNOTSUPPORT;
          break;

        case _00011010_:
          EXE_CPY;
          EXE_LDRD;
          EXE_MOV;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00011011_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_MOV;
          INSTNOTSUPPORT;
          break;

        case _00011100_:
          EXE_BIC;
          EXE_LDRD;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00011101_:
          EXE_BIC;
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          INSTNOTSUPPORT;
          break;

        case _00011110_:
          EXE_LDRD;
          EXE_MVN;
          EXE_STRD;
          EXE_STRH;
          INSTNOTSUPPORT;
          break;

        case _00011111_:
          EXE_LDRH;
          EXE_LDRSB;
          EXE_LDRSH;
          EXE_MVN;
          INSTNOTSUPPORT;
          break;

        case _00100000_:
          EXE_AND;
          INSTNOTSUPPORT;
          break;

        case _00100001_:
          EXE_AND;
          INSTNOTSUPPORT;
          break;

        case _00100010_:
          EXE_EOR;
          INSTNOTSUPPORT;
          break;

        case _00100011_:
          EXE_EOR;
          INSTNOTSUPPORT;
          break;

        case _00100100_:
          EXE_SUB;
          INSTNOTSUPPORT;
          break;

        case _00100101_:
          EXE_SUB;
          INSTNOTSUPPORT;
          break;

        case _00100110_:
          EXE_RSB;
          INSTNOTSUPPORT;
          break;

        case _00100111_:
          EXE_RSB;
          INSTNOTSUPPORT;
          break;

        case _00101000_:
          EXE_ADD;
          INSTNOTSUPPORT;
          break;

        case _00101001_:
          EXE_ADD;
          INSTNOTSUPPORT;
          break;

        case _00101010_:
          EXE_ADC;
          INSTNOTSUPPORT;
          break;

        case _00101011_:
          EXE_ADC;
          INSTNOTSUPPORT;
          break;

        case _00101100_:
          EXE_SBC;
          INSTNOTSUPPORT;
          break;

        case _00101101_:
          EXE_SBC;
          INSTNOTSUPPORT;
          break;

        case _00101110_:
          EXE_RSC;
          INSTNOTSUPPORT;
          break;

        case _00101111_:
          EXE_RSC;
          INSTNOTSUPPORT;
          break;

        case _00110000_:
          INSTNOTSUPPORT;
          break;

        case _00110001_:
          EXE_TST;
          INSTNOTSUPPORT;
          break;

        case _00110010_:
          EXE_MSR;
          INSTNOTSUPPORT;
          break;

        case _00110011_:
          EXE_TEQ;
          INSTNOTSUPPORT;
          break;

        case _00110100_:
          INSTNOTSUPPORT;
          break;

        case _00110101_:
          EXE_CMP;
          INSTNOTSUPPORT;
          break;

        case _00110110_:
          EXE_MSR;
          INSTNOTSUPPORT;
          break;

        case _00110111_:
          EXE_CMN;
          INSTNOTSUPPORT;
          break;

        case _00111000_:
          EXE_ORR;
          INSTNOTSUPPORT;
          break;

        case _00111001_:
          EXE_ORR;
          INSTNOTSUPPORT;
          break;

        case _00111010_:
          EXE_MOV;
          INSTNOTSUPPORT;
          break;

        case _00111011_:
          EXE_MOV;
          INSTNOTSUPPORT;
          break;

        case _00111100_:
          EXE_BIC;
          INSTNOTSUPPORT;
          break;

        case _00111101_:
          EXE_BIC;
          INSTNOTSUPPORT;
          break;

        case _00111110_:
          EXE_MVN;
          INSTNOTSUPPORT;
          break;

        case _00111111_:
          EXE_MVN;
          INSTNOTSUPPORT;
          break;

        case _01000000_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01000001_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01000010_:
          EXE_STR;
          EXE_STRT;
          INSTNOTSUPPORT;
          break;

        case _01000011_:
          EXE_LDR;
          EXE_LDRT;
          INSTNOTSUPPORT;
          break;

        case _01000100_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01000101_:
          EXE_LDRB;
          INSTNOTSUPPORT;
          break;

        case _01000110_:
          EXE_STRB;
          EXE_STRBT;
          INSTNOTSUPPORT;
          break;

        case _01000111_:
          EXE_LDRB;
          EXE_LDRBT;
          INSTNOTSUPPORT;
          break;

        case _01001000_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01001001_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01001010_:
          EXE_STR;
          EXE_STRT;
          INSTNOTSUPPORT;
          break;

        case _01001011_:
          EXE_LDR;
          EXE_LDRT;
          INSTNOTSUPPORT;
          break;

        case _01001100_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01001101_:
          EXE_LDRB;
          INSTNOTSUPPORT;
          break;

        case _01001110_:
          EXE_STRB;
          EXE_STRBT;
          INSTNOTSUPPORT;
          break;

        case _01001111_:
          EXE_LDRB;
          EXE_LDRBT;
          INSTNOTSUPPORT;
          break;

        case _01010000_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01010001_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01010010_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01010011_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01010100_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01010101_:
          EXE_LDRB;
          EXE_PLD;
          INSTNOTSUPPORT;
          break;

        case _01010110_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01010111_:
          EXE_LDRB;
          INSTNOTSUPPORT;
          break;

        case _01011000_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01011001_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01011010_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01011011_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01011100_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01011101_:
          EXE_LDRB;
          EXE_PLD;
          INSTNOTSUPPORT;
          break;

        case _01011110_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01011111_:
          EXE_LDRB;
          INSTNOTSUPPORT;
          break;

        case _01100000_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01100001_:
          EXE_LDR;
          EXE_SADD16;
          EXE_SADD8;
          EXE_SADDSUBX;
          EXE_SSUB16;
          EXE_SSUB8;
          EXE_SSUBADDX;
          INSTNOTSUPPORT;
          break;

        case _01100010_:
          EXE_QADD16;
          EXE_QADD8;
          EXE_QADDSUBX;
          EXE_QSUB16;
          EXE_QSUB8;
          EXE_QSUBADDX;
          EXE_STR;
          EXE_STRT;
          INSTNOTSUPPORT;
          break;

        case _01100011_:
          EXE_LDR;
          EXE_LDRT;
          EXE_SHADD16;
          EXE_SHADD8;
          EXE_SHADDSUBX;
          EXE_SHSUB16;
          EXE_SHSUB8;
          EXE_SHSUBADDX;
          INSTNOTSUPPORT;
          break;

        case _01100100_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01100101_:
          EXE_LDRB;
          EXE_UADD16;
          EXE_UADD8;
          EXE_UADDSUBX;
          EXE_USUB16;
          EXE_USUB8;
          EXE_USUBADDX;
          INSTNOTSUPPORT;
          break;

        case _01100110_:
          EXE_STRB;
          EXE_STRBT;
          EXE_UQADD16;
          EXE_UQADD8;
          EXE_UQADDSUBX;
          EXE_UQSUB16;
          EXE_UQSUB8;
          EXE_UQSUBADDX;
          INSTNOTSUPPORT;
          break;

        case _01100111_:
          EXE_LDRB;
          EXE_LDRBT;
          EXE_UHADD16;
          EXE_UHADD8;
          EXE_UHADDSUBX;
          EXE_UHSUB16;
          EXE_UHSUB8;
          EXE_UHSUBADDX;
          INSTNOTSUPPORT;
          break;

        case _01101000_:
          EXE_PKHBT;
          EXE_PKHTB;
          EXE_SEL;
          EXE_STR;
          EXE_SXTAB16;
          EXE_SXTB16;
          INSTNOTSUPPORT;
          break;

        case _01101001_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01101010_:
          EXE_SSAT;
          EXE_SSAT16;
          EXE_STR;
          EXE_STRT;
          EXE_SXTAB;
          EXE_SXTB;
          INSTNOTSUPPORT;
          break;

        case _01101011_:
          EXE_LDR;
          EXE_LDRT;
          EXE_REV;
          EXE_REV16;
          EXE_SSAT;
          EXE_SXTAH;
          EXE_SXTH;
          INSTNOTSUPPORT;
          break;

        case _01101100_:
          EXE_STRB;
          EXE_UXTAB16;
          EXE_UXTB16;
          INSTNOTSUPPORT;
          break;

        case _01101101_:
          EXE_LDRB;
          INSTNOTSUPPORT;
          break;

        case _01101110_:
          EXE_STRB;
          EXE_STRBT;
          EXE_USAT;
          EXE_USAT16;
          EXE_UXTAB;
          EXE_UXTB;
          INSTNOTSUPPORT;
          break;

        case _01101111_:
          EXE_LDRB;
          EXE_LDRBT;
          EXE_REVSH;
          EXE_USAT;
          EXE_UXTAH;
          EXE_UXTH;
          INSTNOTSUPPORT;
          break;

        case _01110000_:
          EXE_SMLAD;
          EXE_SMLSD;
          EXE_SMUAD;
          EXE_SMUSD;
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01110001_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01110010_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01110011_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01110100_:
          EXE_SMLALD;
          EXE_SMLSLD;
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01110101_:
          EXE_LDRB;
          EXE_PLD;
          EXE_SMMLA;
          EXE_SMMLS;
          EXE_SMMUL;
          INSTNOTSUPPORT;
          break;

        case _01110110_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01110111_:
          EXE_LDRB;
          INSTNOTSUPPORT;
          break;

        case _01111000_:
          EXE_STR;
          EXE_USAD8;
          EXE_USADA8;
          INSTNOTSUPPORT;
          break;

        case _01111001_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01111010_:
          EXE_STR;
          INSTNOTSUPPORT;
          break;

        case _01111011_:
          EXE_LDR;
          INSTNOTSUPPORT;
          break;

        case _01111100_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01111101_:
          EXE_LDRB;
          EXE_PLD;
          INSTNOTSUPPORT;
          break;

        case _01111110_:
          EXE_STRB;
          INSTNOTSUPPORT;
          break;

        case _01111111_:
          EXE_LDRB;
          INSTNOTSUPPORT;
          break;

        case _10000000_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10000001_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10000010_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10000011_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10000100_:
          EXE_SRS;
          EXE_STM2;
          INSTNOTSUPPORT;
          break;

        case _10000101_:
          EXE_LDM2;
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10000110_:
          EXE_SRS;
          INSTNOTSUPPORT;
          break;

        case _10000111_:
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10001000_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10001001_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10001010_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10001011_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10001100_:
          EXE_SRS;
          EXE_STM2;
          INSTNOTSUPPORT;
          break;

        case _10001101_:
          EXE_LDM2;
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10001110_:
          EXE_SRS;
          INSTNOTSUPPORT;
          break;

        case _10001111_:
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10010000_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10010001_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10010010_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10010011_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10010100_:
          EXE_SRS;
          EXE_STM2;
          INSTNOTSUPPORT;
          break;

        case _10010101_:
          EXE_LDM2;
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10010110_:
          EXE_SRS;
          INSTNOTSUPPORT;
          break;

        case _10010111_:
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10011000_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10011001_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10011010_:
          EXE_STM1;
          INSTNOTSUPPORT;
          break;

        case _10011011_:
          EXE_LDM1;
          EXE_RFE;
          INSTNOTSUPPORT;
          break;

        case _10011100_:
          EXE_SRS;
          EXE_STM2;
          INSTNOTSUPPORT;
          break;

        case _10011101_:
          EXE_LDM2;
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10011110_:
          EXE_SRS;
          INSTNOTSUPPORT;
          break;

        case _10011111_:
          EXE_LDM3;
          INSTNOTSUPPORT;
          break;

        case _10100000_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10100001_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10100010_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10100011_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10100100_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10100101_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10100110_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10100111_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101000_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101001_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101010_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101011_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101100_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101101_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101110_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10101111_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110000_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110001_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110010_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110011_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110100_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110101_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110110_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10110111_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111000_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111001_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111010_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111011_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111100_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111101_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111110_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _10111111_:
          EXE_BBL;
          EXE_BLX1;
          INSTNOTSUPPORT;
          break;

        case _11000000_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11000001_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11000010_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11000011_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11000100_:
          EXE_MCRR;
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11000101_:
          EXE_LDC;
          EXE_MRRC;
          INSTNOTSUPPORT;
          break;

        case _11000110_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11000111_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11001000_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11001001_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11001010_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11001011_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11001100_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11001101_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11001110_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11001111_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11010000_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11010001_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11010010_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11010011_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11010100_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11010101_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11010110_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11010111_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11011000_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11011001_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11011010_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11011011_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11011100_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11011101_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11011110_:
          EXE_STC;
          INSTNOTSUPPORT;
          break;

        case _11011111_:
          EXE_LDC;
          INSTNOTSUPPORT;
          break;

        case _11100000_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11100001_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11100010_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11100011_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11100100_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11100101_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11100110_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11100111_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11101000_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11101001_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11101010_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11101011_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11101100_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11101101_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11101110_:
          EXE_CDP;
          EXE_MCR;
          INSTNOTSUPPORT;
          break;

        case _11101111_:
          EXE_CDP;
          EXE_MRC;
          INSTNOTSUPPORT;
          break;

        case _11110000_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11110001_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11110010_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11110011_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11110100_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11110101_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11110110_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11110111_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111000_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111001_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111010_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111011_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111100_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111101_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111110_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        case _11111111_:
          EXE_SWI;
          INSTNOTSUPPORT;
          break;

        default:
          PROGRAM_ERROR;
          break;
      }
    }

  return 0;
}
