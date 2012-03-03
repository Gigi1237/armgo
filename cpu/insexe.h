#ifndef INSEXE_H_
#define INSEXE_H_


/* instruction error */
#define UNPREDICTABLE  { printf("Unpredictable instruction!\n"); }
#define INSTNOTSUPPORT { printf("Instruction not support in this version!\n"); }


#define INSNEXTADDRESS ( TFLAG ? ( cpu.pipeline16[1].address ) : ( cpu.pipeline32[1].address ) )
#define PIPELINEREFRESHED (cpu.pipelineRefreshed)



#define COND (BITS(31, 28) != _1111_)


/* Condition code values.  */
#define EQ 0
#define NE 1
#define CS 2
#define CC 3
#define MI 4
#define PL 5
#define VS 6
#define VC 7
#define HI 8
#define LS 9
#define GE 10
#define LT 11
#define GT 12
#define LE 13
#define AL 14
#define NV 15

#define POS(i) ( (~(i)) >> 31 )
#define NEG(i) ( (i) >> 31 )


/* Addressing mode 1 -- Data-processing Operands */
#define ADDRESSING_MODE1_IMM         8 /* A5.1.3 */
#define ADDRESSING_MODE1_REGORLSLIMM 0 /* A5.1.4 and A5.1.5 */
#define ADDRESSING_MODE1_LSLREG      1 /* A5.1.6 */
#define ADDRESSING_MODE1_LSRIMM      2 /* A5.1.7 */
#define ADDRESSING_MODE1_LSRREG      3 /* A5.1.8 */
#define ADDRESSING_MODE1_ASRIMM      4 /* A5.1.9 */
#define ADDRESSING_MODE1_ASRREG      5 /* A5.1.10 */
#define ADDRESSING_MODE1_RORIMMOREXT 6 /* A5.1.11 and A5.1.13 */
#define ADDRESSING_MODE1_RORREG      7 /* A5.1.12 */


/* Addressing mode 2 -- Load and Store Word or Unsigned Byte */
#define ADDRESSING_MODE2_IMMOFF             0x10 /* A5.2.2 */
#define ADDRESSING_MODE2_SCALEDREGOFF       0x30 /* A5.2.3 and A5.2.4 */
#define ADDRESSING_MODE2_IMMPREINDEX        0x12 /* A5.2.5 */
#define ADDRESSING_MODE2_SCALEDREGPREINDEX  0x32 /* A5.2.6 and A5.2.7 */
#define ADDRESSING_MODE2_IMMPOSTINDEX       0x00 /* A5.2.8 */
#define ADDRESSING_MODE2_SCALEDREGPOSTINDEX 0x20 /* A5.2.9 and A5.2.10 */


/* Addressing mode 3 -- Miscellaneous Loads and Stores */
#define ADDRESSING_MODE3_IMMOFF             0x14 /* A5.3.2 */
#define ADDRESSING_MODE3_REGOFF             0x10 /* A5.3.3 */
#define ADDRESSING_MODE3_IMMPREINDEX        0x16 /* A5.3.4 */
#define ADDRESSING_MODE3_REGPREINDEX        0x12 /* A5.3.5 */
#define ADDRESSING_MODE3_IMMPOSTINDEX       0x04 /* A5.3.6 */
#define ADDRESSING_MODE3_REGPOSTINDEX       0x00 /* A5.3.7 */


/* Addressing mode 4 -- Load and Store Multiple */
#define ADDRESSING_MODE4_LSM_DA  0 /* A5.4.4 */
#define ADDRESSING_MODE4_LSM_IA  1 /* A5.4.2 */
#define ADDRESSING_MODE4_LSM_DB  2 /* A5.4.5 */
#define ADDRESSING_MODE4_LSM_IB  3 /* A5.4.3 */


/* Addressing mode 5 -- Load and Store Coprocessor */
#define ADDRESSING_MODE5_IMMOFF       0x10 /* A5.5.2 */
#define ADDRESSING_MODE5_IMMPREINDEX  0x12 /* A5.5.3 */
#define ADDRESSING_MODE5_IMMPOSTINDEX 0x02 /* A5.5.4 */
#define ADDRESSING_MODE5_UNINDEXED    0x00 /* A5.5.5 */










/* 0 ADC */
#define EXE_ADC                                                   \
if(COND)                                                          \
{                                                                 \
  DATA_PROCESSING_OPERANDS;                                       \
  rd = rn + shifter_operand + CFLAG;                              \
  writeReg(d, rd);                                                \
  int sbit = BIT(20);                                             \
  if( (sbit == 1) && (d == 15) )                                  \
  {                                                               \
    if(CurrentModeHasSPSR())                                      \
    {                                                             \
      CPSR_CUR = SPSR_CUR;                                        \
    }                                                             \
	else                                                          \
	{                                                             \
      UNPREDICTABLE;                                              \
	}                                                             \
  }                                                               \
  else if(sbit == 1)                                              \
  {                                                               \
    ASSIGNN(GETBIT(rd, 31));                                      \
    ASSIGNZ((rd==0) ? 1 : 0);                                     \
    ASSIGNC(CarryFromCflag(rn, shifter_operand, CFLAG));          \
    ASSIGNV(OverflowFromCflag_add(rn, shifter_operand, CFLAG));   \
  }                                                               \
  break;                                                          \
}


/* 1 ADD */
#define EXE_ADD                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = rn + shifter_operand;                             \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
    else                                                 \
    {                                                    \
      UNPREDICTABLE;                                     \
    }                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
    ASSIGNC(CarryFrom(rn, shifter_operand));             \
	ASSIGNV(OverflowFrom_add(rn, shifter_operand));      \
  }                                                      \
  break;                                                 \
}

/* 2 AND */
#define EXE_AND                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = rn & shifter_operand;                             \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
	else                                                 \
	{                                                    \
      UNPREDICTABLE;                                     \
    }                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
	ASSIGNC(shifter_carry_out);                          \
  }                                                      \
  break;                                                 \
}


/* 3 BBL */
#define EXE_BBL                                          \
if(COND)                                                 \
{                                                        \
  int lbit = BIT(24);                                    \
  if(lbit == 1)                                          \
  {                                                      \
    LR_CUR = INSNEXTADDRESS;                             \
  }                                                      \
  writeReg(15, PC + (SignExtend(BITS(23, 0), 23) << 2)); \
  break;                                                 \
}


/* 4 BIC */
#define EXE_BIC                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = rn & (~shifter_operand);                          \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
	else                                                 \
	{                                                    \
      UNPREDICTABLE;                                     \
	}                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
    ASSIGNC(shifter_carry_out);                          \
  }                                                      \
  break;                                                 \
}



/* 5 BKPT */
#define EXE_BKPT                                         \
if( (BITS(31, 28) == _1110_) && (BITS(7, 4) == _0111_) ) \
{                                                        \
  Reg_ABT(14) = INSNEXTADDRESS;                          \
  SPSR_ABT = CPSR_CUR;                                   \
  ASSIGNBITS(CPSR_CUR, 4, 0, _10111_);                   \
                                                         \
  ASSIGNBIT(CPSR_CUR, 5, 0);                             \
  ASSIGNBIT(CPSR_CUR, 7, 1);                             \
  ASSIGNBIT(CPSR_CUR, 8, 1);                             \
  ASSIGNBIT(CPSR_CUR, 9, get_CP15_reg1_EEbit);           \
                                                         \
  if(high_vectors_configured())                          \
  {                                                      \
	writeReg(15, 0xffff000c);                            \
  }                                                      \
  else                                                   \
  {                                                      \
	writeReg(15, 0x0000000c);                            \
  }                                                      \
                                                         \
  break;                                                 \
}


/* 6 BLX1 */
#define EXE_BLX1                                                         \
if(BITS(31, 28) == _1111_)                                               \
{                                                                        \
  int hbit = BIT(24);                                                    \
  LR_CUR = INSNEXTADDRESS;                                               \
  TFLAG = 1;                                                             \
  writeReg(15, PC + (SignExtend(BITS(23, 0), 23) << 2) + (hbit << 1));   \
  break;                                                                 \
}

/* 7 BLX2 */
#define EXE_BLX2                                       \
if( COND && (BITS(7, 4) == _0011_) )                   \
{                                                      \
  rm = BITS(3, 0);                                     \
  INT32 target = rm;                                   \
  LR_CUR = INSNEXTADDRESS;                             \
  TFLAG = GETBIT(target, 0);                           \
  writeReg(15, target & 0xfffffffe);                   \
  break;                                               \
}

/* 8 BX */
#define EXE_BX                                           \
if( COND && (BITS(7, 4) == _0001_) )                     \
{                                                        \
  rm = BITS(3, 0);                                       \
  TFLAG = GETBIT(rm , 0);                                \
  writeReg(15, rm & 0xfffffffe);                         \
  break;                                                 \
}

/* 9 BXJ */
#define EXE_BXJ                                        \
if( COND && (BITS(7, 4) == _0010_) )                   \
{                                                      \
  INSTNOTSUPPORT("BXJ");                               \
}


/* 10 CDP */
#define EXE_CDP                                          \
if( COND && (BIT(4) == 0) )                              \
{                                                        \
  INSTNOTSUPPORT("CDP");                                 \
}

/* 11 CLZ */
#define EXE_CLZ                                          \
if( COND && (BITS(7, 4) == _0001_) )                     \
{                                                        \
  rm = Reg(BITS(3, 0));                                  \
                                                         \
  if(rm == 0)                                            \
  {                                                      \
	writeReg(BITS(15, 12), 32);                          \
  }                                                      \
  else                                                   \
  {                                                      \
    int position;                                        \
    for(position=31; position>=0; position--)            \
    {                                                    \
	  if(GETBIT(rm, position))                           \
		break;                                           \
    }                                                    \
	writeReg(BITS(15, 12), 31 - position);               \
  }                                                      \
                                                         \
  break;                                                 \
}

/* 12 CMN */
#define EXE_CMN                                                         \
if( COND )                                                              \
{                                                                       \
  INT32 alu_out;                                                        \
  DATA_PROCESSING_OPERANDS;                                             \
  alu_out = rn + shifter_operand;                                       \
  ASSIGNN(GETBIT(alu_out, 31));                                         \
  ASSIGNZ((alu_out==0) ? 1 : 0);                                        \
  ASSIGNC(CarryFrom(rn, shifter_operand));                              \
  ASSIGNV(OverflowFrom_add(rn, shifter_operand));                       \
  break;                                                                \
}

/* 13 CMP */
#define EXE_CMP                                                         \
if( COND )                                                              \
{                                                                       \
  INT32 alu_out;                                                        \
  DATA_PROCESSING_OPERANDS;                                             \
  alu_out = rn - shifter_operand;                                       \
  ASSIGNN(GETBIT(alu_out, 31));                                         \
  ASSIGNZ((alu_out==0) ? 1 : 0);                                        \
  ASSIGNC(!BorrowFrom(rn, shifter_operand));                            \
  ASSIGNV(OverflowFrom_sub(rn, shifter_operand));                       \
  break;                                                                \
}

/* 14 CPS */
#define EXE_CPS                                                      \
if( (BITS(31, 28) == _1111_) && (BIT(16) == 0) && (BIT(5) == 0) )    \
{                                                                    \
  int imod = BITS(19, 18);                                           \
  int mmod = BIT(17);                                                \
  int mode = BITS(4, 0);                                             \
                                                                     \
  if(InAPrivilegedMode())                                            \
  {                                                                  \
    if(GETBIT(imod, 1) == 1)                                         \
    {                                                                \
      if(BIT(8) == 1)                                                \
    	ASSIGNBIT(CPSR_CUR, 8, GETBIT(imod, 0));                     \
      if(BIT(7) == 1)                                                \
    	ASSIGNBIT(CPSR_CUR, 7, GETBIT(imod, 0));                     \
      if(BIT(6) == 1)                                                \
    	ASSIGNBIT(CPSR_CUR, 6, GETBIT(imod, 0));                     \
    }                                                                \
    if(mmod == 1)                                                    \
    {                                                                \
      ASSIGNBITS(CPSR_CUR, 4, 0, mode);                              \
    }                                                                \
  }                                                                  \
                                                                     \
  break;                                                             \
}

/* 15 CPY */
#define EXE_CPY                                  \
if( COND && (BITS(11, 4) == _00000000_) )        \
{                                                \
  writeReg(BITS(15, 12), Reg(BITS(3, 0)));       \
  break;                                         \
}

/* 16 EOR */
#define EXE_EOR                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = rn ^ shifter_operand;                             \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
	else                                                 \
	{                                                    \
      UNPREDICTABLE;                                     \
    }                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
	ASSIGNC(shifter_carry_out);                          \
  }                                                      \
  break;                                                 \
}


/* 17 LDC */
#define EXE_LDC                                  \
if( COND )                                       \
{                                                \
  INSTNOTSUPPORT;                                \
  break;                                         \
}

/* 18 LDM1 */
#define EXE_LDM1                                 \
if( COND )                                       \
{                                                \
  int i;                                         \
  LOAD_STORE_MULTIPLE;                           \
                                                 \
  INT32 address = start_address;                 \
  for(i=0; i<=14; ++i)                           \
  {                                              \
	if(GETBIT(register_list, i) == 1)            \
	{                                            \
	  Reg(i) = readMemoryWord(address);          \
	  address += 4;                              \
	}                                            \
  }                                              \
  if(GETBIT(register_list, 15) == 1)             \
  {                                              \
	INT32 value = readMemoryWord(address);       \
    writeReg(15, value & 0xfffffffc);            \
	address += 4;                                \
  }                                              \
  if(end_address != (address - 4))               \
  {                                              \
	PROGRAM_ERROR;                               \
  }                                              \
  break;                                         \
}


/* 19 LDM2 */
#define EXE_LDM2                                               \
if( COND && (BIT(15) == 0) )                                   \
{                                                              \
  int i;                                                       \
  LOAD_STORE_MULTIPLE;                                         \
                                                               \
  INT32 address = start_address;                               \
  for(i=0; i<=14; ++i)                                         \
  {                                                            \
	if(GETBIT(register_list, i) == 1)                          \
	{                                                          \
	  Reg_USR(i) = readMemoryWord(address);                    \
	  address += 4;                                            \
	}                                                          \
  }                                                            \
  if(end_address != (address - 4))                             \
  {                                                            \
	PROGRAM_ERROR;                                             \
  }                                                            \
  break;                                                       \
}

/* 20 LDM3 */
#define EXE_LDM3                                               \
if( COND && (BIT(15) == 1) )                                   \
{                                                              \
  int i;                                                       \
  LOAD_STORE_MULTIPLE;                                         \
                                                               \
  INT32 address = start_address;                               \
  for(i=0; i<=14; ++i)                                         \
  {                                                            \
	if(GETBIT(register_list, i) == 1)                          \
	{                                                          \
	  Reg(i) = readMemoryWord(address);                        \
	  address += 4;                                            \
	}                                                          \
  }                                                            \
  if(CurrentModeHasSPSR())                                     \
  {                                                            \
	CPSR_CUR = SPSR_CUR;                                       \
  }                                                            \
  else                                                         \
  {                                                            \
	UNPREDICTABLE;                                             \
  }                                                            \
  INT32 value = readMemoryWord(address);                       \
  writeReg(15, value);                                         \
  address += 4;                                                \
  if(end_address != (address - 4))                             \
  {                                                            \
	PROGRAM_ERROR;                                             \
  }                                                            \
  break;                                                       \
}

/* 21 LDR */
#define EXE_LDR                                                \
if( COND )                                                     \
{                                                              \
  LOAD_STORE_WORD_BYTE;                                        \
                                                               \
  INT32 data = readMemoryWord(address);                        \
  if(BITS(15, 12) == 15)                                       \
  {                                                            \
    writeReg(15, data & 0xfffffffc);                           \
  }                                                            \
  else                                                         \
  {                                                            \
	INT32 rd = data;                                           \
	writeReg(BITS(15, 12), rd);                                \
  }                                                            \
  break;                                                       \
}


/* 22 LDRB */
#define EXE_LDRB                                      \
if( COND )                                            \
{                                                     \
  LOAD_STORE_WORD_BYTE;                               \
                                                      \
  INT32 rd = readMemoryWord(address);                 \
  writeReg(BITS(15, 12), (INT8)(rd & 0xff));          \
  break;                                              \
}

/* 23 LDRBT */
#define EXE_LDRBT                                     \
if( COND )                                            \
{                                                     \
  LOAD_STORE_WORD_BYTE;                               \
                                                      \
  INT32 rd = (INT32)readMemoryByte(address);          \
  writeReg(BITS(15, 12), rd);                         \
  writeReg(BITS(19, 16), address);                    \
  break;                                              \
}

/* 24 LDRD */
#define EXE_LDRD                                                \
if(COND && (BITS(7, 4) == _1101_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 25 LDREX */
#define EXE_LDREX                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 26 LDRH */
#define EXE_LDRH                                      \
if( COND && (BITS(7, 4) == _1011_) )                  \
{                                                     \
  LOAD_STORE_MISCELLANEOUS;                           \
                                                      \
  INT16 data = readMemoryHalfword(address);           \
  writeReg(BITS(15, 12), (INT32)data);                \
  break;                                              \
}

/* 27 LDRSB */
#define EXE_LDRSB                                     \
if( COND && (BITS(7, 4) == _1101_) )                  \
{                                                     \
  LOAD_STORE_MISCELLANEOUS;                           \
                                                      \
  INT8 data = readMemoryByte(address);                \
  writeReg(BITS(15, 12), SignExtend(data, 7));        \
  break;                                              \
}

/* 28 LDRSH */
#define EXE_LDRSH                                     \
if( COND && (BITS(7, 4) == _1111_) )                  \
{                                                     \
  LOAD_STORE_MISCELLANEOUS;                           \
                                                      \
  INT16 data = readMemoryHalfword(address);           \
  writeReg(BITS(15, 12), SignExtend(data, 15));       \
  break;                                              \
}

/* 29 LDRT */
#define EXE_LDRT                                      \
if( COND )                                            \
{                                                     \
  LOAD_STORE_WORD_BYTE;                               \
                                                      \
  INT32 rd = readMemoryWord(address);                 \
  writeReg(BITS(15, 12), rd);                         \
  break;                                              \
}

/* 30 MCR */
#define EXE_MCR                                                          \
if( COND && (BIT(4) == 1) )                                              \
{                                                                        \
  int cp_num = BITS(11, 8);                                              \
  Coprocessor(cp_num, BITS(19, 16)) = Reg(BITS(15, 12));                 \
  break;                                                                 \
}

/* 31 MCRR */
#define EXE_MCRR                                                \
if( COND )                                                      \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 32 MLA */
#define EXE_MLA                                                                   \
if( COND && (BITS(7, 4) == _1001_) )                                              \
{                                                                                 \
  INT64 result;                                                                   \
  int d = BITS(19, 16);                                                           \
  int sbit = BIT(20);                                                             \
                                                                                  \
  result = multiplyUnsigned32((INT64)Reg(BITS(11, 8)), (INT64)Reg(BITS(3, 0)));   \
  result += (INT64)Reg(BITS(15, 12));                                             \
  rd = (INT32)(result & 0xffffffff);                                              \
  writeReg(d, rd);                                                                \
  if(sbit == 1)                                                                   \
  {                                                                               \
    ASSIGNN(GETBIT(rd, 31));                                                      \
    ASSIGNZ( (rd==0) ? 1 : 0 );                                                   \
  }                                                                               \
                                                                                  \
  break;                                                                          \
}

/* 33 MOV */
#define EXE_MOV                                                 \
if( COND )                                                      \
{                                                               \
  int sbit = BIT(20);                                           \
  DATA_PROCESSING_OPERANDS;                                     \
  rd = shifter_operand;                                         \
  writeReg(d, rd);                                              \
  if( (sbit == 1) && (d == 15) )                                \
  {                                                             \
    if(CurrentModeHasSPSR())                                    \
    {                                                           \
      CPSR_CUR = SPSR_CUR;                                      \
    }                                                           \
    else                                                        \
    {                                                           \
      UNPREDICTABLE;                                            \
    }                                                           \
  }                                                             \
  else if(sbit == 1)                                            \
  {                                                             \
    ASSIGNN(GETBIT(rd, 31));                                    \
    ASSIGNZ((rd==0) ? 1 : 0);                                   \
    ASSIGNC(shifter_carry_out);                                 \
  }                                                             \
  break;                                                        \
}

/* 34 MRC */
#define EXE_MRC                                                 \
if( COND && (BIT(4) == 1) )                                     \
{                                                               \
  int cp_num = BITS(11, 8);                                     \
  int d = BITS(15, 12);                                         \
  INT32 data = Coprocessor(cp_num, BITS(19, 16));               \
  INT32 rd;                                                     \
                                                                \
  if(d == 15)                                                   \
  {                                                             \
    ASSIGNN(GETBIT(data, 31));                                  \
    ASSIGNZ(GETBIT(data, 30));                                  \
    ASSIGNC(GETBIT(data, 29));                                  \
    ASSIGNV(GETBIT(data, 28));                                  \
  }                                                             \
  else                                                          \
  {                                                             \
    rd = data;                                                  \
    writeReg(d, rd);                                            \
  }                                                             \
  break;                                                        \
}

/* 35 MRRC */
#define EXE_MRRC                                                \
if( COND )                                                      \
{                                                               \
  INSTNOTSUPPORT;                                               \
}

/* 36 MRS */
#define EXE_MRS                                                 \
if( COND )                                                      \
{                                                               \
  int rbit = BIT(22);                                           \
  int d = BITS(15, 12);                                         \
  INT32 rd;                                                     \
                                                                \
  if(rbit == 1)                                                 \
  {                                                             \
    rd = SPSR_CUR;                                              \
  }                                                             \
  else                                                          \
  {                                                             \
    rd = CPSR_CUR;                                              \
  }                                                             \
                                                                \
  writeReg(d, rd);                                              \
  break;                                                        \
}

/* 37 MSR */
#define EXE_MSR                                                                    \
if( COND && ( (BIT(25) == 1) || ( (BIT(25) == 0) && (BITS(7, 4) == _0000_) ) ) )   \
{                                                                                  \
  INT32 rm               = Reg(BITS(3, 0));                                        \
  INT32 _8_bit_immediate = BITS(7, 0);                                             \
  INT32 rotate_imm       = BITS(11, 8);                                            \
  INT32 field_mask       = BITS(19, 16);                                           \
  int rbit = BIT(22);                                                              \
  INT32 byte_mask;                                                                 \
  INT32 operand;                                                                   \
  INT32 mask;                                                                      \
                                                                                   \
  /* Architecture version 4 */                                                     \
  INT32 UnallocMask = 0x0fffff20;                                                  \
  INT32 UserMask    = 0xf0000000;                                                  \
  INT32 PrivMask    = 0x0000000f;                                                  \
  INT32 StateMask   = 0x00000000;                                                  \
                                                                                   \
  if( BIT(25) == 1 )                                                               \
  {                                                                                \
    operand = ROR8(_8_bit_immediate, rotate_imm * 2);                             \
  }                                                                                \
  else                                                                             \
  {                                                                                \
	operand = rm;                                                                  \
  }                                                                                \
  if( (operand & UnallocMask) != 0 )                                               \
  {                                                                                \
    UNPREDICTABLE;                                                                 \
  }                                                                                \
  byte_mask = ( ( GETBIT(field_mask, 0) ? 0x000000ff : 0x00000000 ) |               \
		        ( GETBIT(field_mask, 1) ? 0x0000ff00 : 0x00000000 ) |               \
		        ( GETBIT(field_mask, 2) ? 0x00ff0000 : 0x00000000 ) |               \
		        ( GETBIT(field_mask, 3) ? 0xff000000 : 0x00000000 ) );              \
  if(rbit == 0)                                                                    \
  {                                                                                \
    if(InAPrivilegedMode())                                                        \
    {                                                                              \
      if( (operand & StateMask) != 0 )                                             \
      {                                                                            \
        UNPREDICTABLE;                                                             \
      }                                                                            \
      else                                                                         \
      {                                                                            \
    	mask = ( byte_mask & (UserMask | PrivMask) );                              \
      }                                                                            \
    }                                                                              \
    else                                                                           \
    {                                                                              \
      mask = ( byte_mask & UserMask );                                             \
    }                                                                              \
    CPSR_CUR = ( (CPSR_CUR & (~mask)) | (operand & mask) );                        \
  }                                                                                \
  else                                                                             \
  {                                                                                \
	if(CurrentModeHasSPSR())                                                       \
	{                                                                              \
	  mask = ( byte_mask & (UserMask | PrivMask | StateMask) );                    \
	  SPSR_CUR = ( ( SPSR_CUR & (~mask) ) | (operand & mask) );                    \
	}                                                                              \
	else                                                                           \
	{                                                                              \
	  UNPREDICTABLE;                                                               \
	}                                                                              \
  }                                                                                \
  break;                                                                           \
}


/* 38 MUL */
#define EXE_MUL                                                 \
if( COND && (BITS(7, 4) == _1001_) )                            \
{                                                               \
  INT64 result;                                                 \
  int s = BITS(11,  8);                                         \
  int m = BITS( 3,  0);                                         \
  int d = BITS(19, 16);                                         \
  int sbit = BIT(20);                                           \
                                                                \
  result = multiplyUnsigned32((INT64)Reg(s), (INT64)Reg(m));    \
  rd = (INT32)(result & 0xffffffff);                            \
  writeReg(d, rd);                                              \
  if(sbit == 1)                                                 \
  {                                                             \
    ASSIGNN(GETBIT(rd, 31));                                    \
    ASSIGNZ( (rd==0) ? 1 : 0 );                                 \
  }                                                             \
                                                                \
  break;                                                        \
}

/* 39 MVN */
#define EXE_MVN                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = ~shifter_operand;                                 \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
	else                                                 \
	{                                                    \
      UNPREDICTABLE;                                     \
    }                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
	ASSIGNC(shifter_carry_out);                          \
  }                                                      \
  break;                                                 \
}



/* 40 ORR */
#define EXE_ORR                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = rn | shifter_operand;                             \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
	else                                                 \
	{                                                    \
      UNPREDICTABLE;                                     \
    }                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
	ASSIGNC(shifter_carry_out);                          \
  }                                                      \
  break;                                                 \
}

/* 41 PKHBT */
#define EXE_PKHBT                                                \
if( COND && (BITS(6, 4) == _001_) )                              \
{                                                                \
  INT32 rn = Reg(BITS(19, 16));                                  \
  INT32 shift_imm = BITS(11, 7);                                 \
  INT32 rm = Reg(BITS(3, 0));                                    \
                                                                 \
  INT32 low  = GETBITS(rn, 15, 0);                               \
  INT32 high = ( GETBITS(LSL32(rm, shift_imm), 31, 16) << 16 );  \
  writeReg(BITS(15, 12), high | low);                            \
  break;                                                         \
}

/* 42 PKHTB */
#define EXE_PKHTB                                               \
if( COND && (BITS(6, 4) == _101_) )                             \
{                                                               \
  INT32 rn = Reg(BITS(19, 16));                                 \
  INT32 shift_imm = BITS(11, 7);                                \
  INT32 rm = Reg(BITS(3, 0));                                   \
  INT32 rd;                                                     \
                                                                \
  if(shift_imm == 0)                                            \
  {                                                             \
	if(GETBIT(rm, 31) == 0)                                     \
	{                                                           \
	  rd = 0x0000;                                              \
	}                                                           \
	else                                                        \
	{                                                           \
	  rd = 0xffff;                                              \
	}                                                           \
  }                                                             \
  else                                                          \
  {                                                             \
	rd = GETBITS(ASR32(rm, shift_imm), 15, 0);                  \
  }                                                             \
  rd |= (GETBITS(rn, 31, 16) << 16);                            \
  writeReg(BITS(15, 12), rd);                                   \
                                                                \
  break;                                                        \
}

/* 43 PLD */
#define EXE_PLD                                                 \
if( (BITS(31, 28) == _1111_) && (BITS(15, 12) == _1111_) )      \
{                                                               \
  /* No change occurs to programmer's model state,
   * but where appropriate, the memory system is
   * signaled that memory accesses to the specified
   * address are likely in the near future.
   */                                                           \
  break;                                                        \
}

/* 44 QADD */
#define EXE_QADD                                                \
if( COND && (BITS(7, 4) == _0101_) )                            \
{                                                               \
  INT32 rn  = Reg(BITS(19, 16));                                \
  INT32 rm  = Reg(BITS( 3,  0));                                \
                                                                \
  writeReg(BITS(15, 12), SignedSat_add(rm, rn, 32));            \
  if(SignedDoesSat_add(rm, rn, 32))                             \
  {                                                             \
	QFLAG = 1;                                                  \
  }                                                             \
                                                                \
  break;                                                        \
}

/* 45 QADD16 */
#define EXE_QADD16                                              \
if( COND && (BITS(7, 4) == _0001_) )                            \
{                                                               \
  INT32 rn = Reg(BITS(19, 16));                                 \
  INT32 rm = Reg(BITS( 3,  0));                                 \
                                                                \
  INT32 rnlo = GETBITS(rn, 15,  0);                             \
  INT32 rnhi = GETBITS(rn, 31, 16);                             \
                                                                \
  INT32 rmlo = GETBITS(rm, 15,  0);                             \
  INT32 rmhi = GETBITS(rm, 31, 16);                             \
                                                                \
  INT32 rdlo = SignedSat_add(rnlo, rmlo, 16);                   \
  INT32 rdhi = SignedSat_add(rnhi, rmhi, 16);                   \
                                                                \
  writeReg(BITS(15, 12) ,(rdlo | (rdhi << 16)));                \
  break;                                                        \
}

/* 46 QADD8 */
#define EXE_QADD8                                                    \
if( COND && (BITS(7, 4) == _1001_) )                                 \
{                                                                    \
  INT32 rn = Reg(BITS(19, 16));                                      \
  INT32 rm = Reg(BITS( 3,  0));                                      \
                                                                     \
  INT32 rnlo = GETBITS(rn,  7,  0);                                  \
  INT32 rmlo = GETBITS(rm,  7,  0);                                  \
                                                                     \
  INT32 rndo = GETBITS(rn, 15,  8);                                  \
  INT32 rmdo = GETBITS(rm, 15,  8);                                  \
                                                                     \
  INT32 rnup = GETBITS(rn, 23, 16);                                  \
  INT32 rmup = GETBITS(rm, 23, 16);                                  \
                                                                     \
  INT32 rnhi = GETBITS(rn, 31, 24);                                  \
  INT32 rmhi = GETBITS(rm, 31, 24);                                  \
                                                                     \
  INT32 rdlo = SignedSat_add(rnlo, rmlo, 8);                         \
  INT32 rddo = SignedSat_add(rndo, rmdo, 8);                         \
  INT32 rdup = SignedSat_add(rnup, rmup, 8);                         \
  INT32 rdhi = SignedSat_add(rnhi, rmhi, 8);                         \
                                                                     \
  INT32 rd = ( rdlo | (rddo << 8) | (rdup << 16) | (rdhi << 24) );   \
  writeReg(BITS(15, 12), rd);                                        \
                                                                     \
  break;                                                             \
}

/* 47 QADDSUBX */
#define EXE_QADDSUBX                                            \
if( COND && (BITS(7, 4) == _0011_) )                            \
{                                                               \
  INT32 rn = Reg(BITS(19, 16));                                 \
  INT32 rm = Reg(BITS( 3,  0));                                 \
                                                                \
  INT32 rnlo = GETBITS(rn, 15,  0);                             \
  INT32 rnhi = GETBITS(rn, 31, 16);                             \
                                                                \
  INT32 rmlo = GETBITS(rm, 15,  0);                             \
  INT32 rmhi = GETBITS(rm, 31, 16);                             \
                                                                \
  INT32 rdlo = SignedSat_sub(rnlo, rmlo, 16);                   \
  INT32 rdhi = SignedSat_add(rnhi, rmhi, 16);                   \
                                                                \
  writeReg(BITS(15, 12) ,(rdlo | (rdhi << 16)));                \
  break;                                                        \
}

/* 48 QDADD */
#define EXE_QDADD                                                          \
if( COND && (BITS(7, 4) == _0101_) )                                       \
{                                                                          \
  INT32 rn  = Reg(BITS(19, 16));                                           \
  INT32 rm  = Reg(BITS( 3,  0));                                           \
                                                                           \
  INT32 tp1 = SignedSat_add(rn, rn,  32);                                  \
  INT32 tp2 = SignedSat_add(rm, tp1, 32);                                  \
                                                                           \
  writeReg(BITS(15, 12), tp2);                                             \
                                                                           \
  if( SignedDoesSat_add(rm, tp1, 32) || SignedDoesSat_add(rn, rn, 32) )    \
  {                                                                        \
	QFLAG = 1;                                                             \
  }                                                                        \
                                                                           \
  break;                                                                   \
}

/* 49 QDSUB */
#define EXE_QDSUB                                                          \
if( COND && (BITS(7, 4) == _0101_) )                                       \
{                                                                          \
  INT32 rn  = Reg(BITS(19, 16));                                           \
  INT32 rm  = Reg(BITS( 3,  0));                                           \
                                                                           \
  INT32 tp1 = SignedSat_add(rn, rn,  32);                                  \
  INT32 tp2 = SignedSat_sub(rm, tp1, 32);                                  \
                                                                           \
  writeReg(BITS(15, 12), tp2);                                             \
                                                                           \
  if( SignedDoesSat_sub(rm, tp1, 32) || SignedDoesSat_add(rn, rn, 32) )    \
  {                                                                        \
	QFLAG = 1;                                                             \
  }                                                                        \
                                                                           \
  break;                                                                   \
}

/* 50 QSUB */
#define EXE_QSUB                                                \
if( COND && (BITS(7, 4) == _0101_) )                            \
{                                                               \
  INT32 rn  = Reg(BITS(19, 16));                                \
  INT32 rm  = Reg(BITS( 3,  0));                                \
                                                                \
  writeReg(BITS(15, 12), SignedSat_sub(rm, rn, 32));            \
                                                                \
  if(SignedDoesSat_sub(rm, rn, 32))                             \
  {                                                             \
	QFLAG = 1;                                                  \
  }                                                             \
                                                                \
  break;                                                        \
}

/* 51 QSUB16 */
#define EXE_QSUB16                                              \
if( COND && (BITS(7, 4) == _0111_) )                            \
{                                                               \
  INT32 rn = Reg(BITS(19, 16));                                 \
  INT32 rm = Reg(BITS( 3,  0));                                 \
                                                                \
  INT32 rnlo = GETBITS(rn, 15,  0);                             \
  INT32 rnhi = GETBITS(rn, 31, 16);                             \
                                                                \
  INT32 rmlo = GETBITS(rm, 15,  0);                             \
  INT32 rmhi = GETBITS(rm, 31, 16);                             \
                                                                \
  INT32 rdlo = SignedSat_sub(rnlo, rmlo, 16);                   \
  INT32 rdhi = SignedSat_sub(rnhi, rmhi, 16);                   \
                                                                \
  writeReg(BITS(15, 12) ,(rdlo | (rdhi << 16)));                \
                                                                \
  break;                                                        \
}

/* 52 QSUB8 */
#define EXE_QSUB8                                               \
if( COND && (BITS(7, 4) == _1111_) )                            \
{                                                               \
  INT32 rn = Reg(BITS(19, 16));                                      \
  INT32 rm = Reg(BITS( 3,  0));                                      \
                                                                     \
  INT32 rnlo = GETBITS(rn,  7,  0);                                  \
  INT32 rmlo = GETBITS(rm,  7,  0);                                  \
                                                                     \
  INT32 rndo = GETBITS(rn, 15,  8);                                  \
  INT32 rmdo = GETBITS(rm, 15,  8);                                  \
                                                                     \
  INT32 rnup = GETBITS(rn, 23, 16);                                  \
  INT32 rmup = GETBITS(rm, 23, 16);                                  \
                                                                     \
  INT32 rnhi = GETBITS(rn, 31, 24);                                  \
  INT32 rmhi = GETBITS(rm, 31, 24);                                  \
                                                                     \
  INT32 rdlo = SignedSat_sub(rnlo, rmlo, 8);                         \
  INT32 rddo = SignedSat_sub(rndo, rmdo, 8);                         \
  INT32 rdup = SignedSat_sub(rnup, rmup, 8);                         \
  INT32 rdhi = SignedSat_sub(rnhi, rmhi, 8);                         \
                                                                     \
  INT32 rd = ( rdlo | (rddo << 8) | (rdup << 16) | (rdhi << 24) );   \
  writeReg(BITS(15, 12), rd);                                        \
                                                                     \
  break;                                                             \
}

/* 53 QSUBADDX */
#define EXE_QSUBADDX                                            \
if( COND && (BITS(7, 4) == _0101_) )                            \
{                                                               \
  INT32 rn = Reg(BITS(19, 16));                                 \
  INT32 rm = Reg(BITS( 3,  0));                                 \
                                                                \
  INT32 rnlo = GETBITS(rn, 15,  0);                             \
  INT32 rnhi = GETBITS(rn, 31, 16);                             \
                                                                \
  INT32 rmlo = GETBITS(rm, 15,  0);                             \
  INT32 rmhi = GETBITS(rm, 31, 16);                             \
  INT32 rd;                                                     \
                                                                \
  INT32 rdlo = SignedSat_add(rnlo, rmlo, 16);                   \
  INT32 rdhi = SignedSat_sub(rnhi, rmhi, 16);                   \
                                                                \
  writeReg(BITS(15, 12) ,(rdlo | (rdhi << 16)));                \
  break;                                                        \
}

/* 54 REV */
#define EXE_REV                                                         \
if( COND && (BITS(7, 4) == _0011_) )                                    \
{                                                                       \
  INT32 rm = Reg(BITS(3, 0));                                           \
                                                                        \
  INT32 rdhi = GETBITS(rm,  7,  0);                                     \
  INT32 rdup = GETBITS(rm, 15,  8);                                     \
  INT32 rddo = GETBITS(rm, 23, 16);                                     \
  INT32 rdlo = GETBITS(rm, 31, 24);                                     \
                                                                        \
  INT32 rd = ( rdlo | (rddo << 8) | (rdup << 16) | (rdhi << 24) );      \
  writeReg(BITS(15, 12), rd);                                           \
                                                                        \
  break;                                                                \
}

/* 55 REV16 */
#define EXE_REV16                                                       \
if( COND && (BITS(7, 4) == _1011_) )                                    \
{                                                                       \
  INT32 rm = Reg(BITS(3, 0));                                           \
                                                                        \
  INT32 rddo = GETBITS(rm,  7,  0);                                     \
  INT32 rdlo = GETBITS(rm, 15,  8);                                     \
  INT32 rdhi = GETBITS(rm, 23, 16);                                     \
  INT32 rdup = GETBITS(rm, 31, 24);                                     \
                                                                        \
  INT32 rd = ( rdlo | (rddo << 8) | (rdup << 16) | (rdhi << 24) );      \
  writeReg(BITS(15, 12), rd);                                           \
                                                                        \
  break;                                                                \
}

/* 56 REVSH */
#define EXE_REVSH                                                       \
if( COND && (BITS(7, 4) == _1011_) )                                    \
{                                                                       \
  INT32 rm = Reg(BITS(3, 0));                                           \
                                                                        \
  INT32 rddo = GETBITS(rm,  7,  0);                                     \
  INT32 rdlo = GETBITS(rm, 15,  8);                                     \
  INT32 rdhi;                                                           \
                                                                        \
  if(GETBIT(rm ,7) == 1)                                                \
  {                                                                     \
    rdhi = 0xffff;                                                      \
  }                                                                     \
  else                                                                  \
  {                                                                     \
    rdhi = 0x0000;                                                      \
  }                                                                     \
                                                                        \
  INT32 rd = ( rdlo | (rddo << 8) | (rdhi << 16) );                     \
  writeReg(BITS(15, 12), rd);                                           \
                                                                        \
  break;                                                                \
}

/* 57 RFE */
#define EXE_RFE                                                          \
if( (BITS(31, 28) == _1111_) && (BITS(11, 8) == _1010_) )                \
{                                                                        \
  /* The following is exactly the same as LOAD_STORE_MULTIPLE,
   * except Number_Of_Set_Bits_In(register_list) = 2  */                 \
                                                                         \
{                                                                        \
  rn = Reg(BITS(19, 16));                                                \
  register_list = BITS(15, 0);                                           \
  /* INT32 temp = Number_Of_Set_Bits_In(register_list) * 4; */           \
  INT32 temp = 2 * 4;                                                    \
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
}                                                                       \
                                                                       \
                                                                      \
                                                                     \
  address = start_address;                                          \
                                                                   \
  INT32 value = readMemoryWord(address);                          \
  if(InAPrivilegedMode())                                        \
  {                                                             \
    CPSR_CUR = readMemoryWord(address + 4);                     \
  }                                                             \
  else                                                          \
  {                                                             \
    UNPREDICTABLE;                                              \
  }                                                             \
                                                                \
  writeReg(15, value);                                          \
                                                                \
  if(end_address != (address + 8))                              \
  {                                                             \
    PROGRAM_ERROR;                                              \
  }                                                             \
                                                                \
  break;                                                        \
}

/* 58 RSB */
#define EXE_RSB                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = shifter_operand - rn;                             \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
    else                                                 \
    {                                                    \
      UNPREDICTABLE;                                     \
    }                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
    ASSIGNC(!BorrowFrom(shifter_operand, rn));           \
	ASSIGNV(OverflowFrom_sub(shifter_operand, rn));      \
  }                                                      \
  break;                                                 \
}

/* 59 RSC */
#define EXE_RSC                                                 \
if(COND)                                                        \
{                                                               \
  DATA_PROCESSING_OPERANDS;                                     \
  rd = shifter_operand - rn - !CFLAG;                           \
  writeReg(d, rd);                                              \
  int sbit = BIT(20);                                           \
  if( (sbit == 1) && (d == 15) )                                \
  {                                                             \
    if(CurrentModeHasSPSR())                                    \
    {                                                           \
      CPSR_CUR = SPSR_CUR;                                      \
    }                                                           \
	else                                                        \
	{                                                           \
      UNPREDICTABLE;                                            \
	}                                                           \
  }                                                             \
  else if(sbit == 1)                                            \
  {                                                             \
    ASSIGNN(GETBIT(rd, 31));                                    \
    ASSIGNZ((rd==0) ? 1 : 0);                                   \
    /* store the original CFLAG */                              \
    INT32 cf = CFLAG;                                           \
    ASSIGNC(!BorrowFromCflag(shifter_operand, rn, !cf));        \
    ASSIGNV(OverflowFromCflag_sub(shifter_operand, rn, !cf));   \
  }                                                             \
  break;                                                        \
}

/* 60 SADD16 */
#define EXE_SADD16                                              \
if(COND && (BITS(7, 4) == _0001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 61 SADD8 */
#define EXE_SADD8                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 62 SADDSUBX */
#define EXE_SADDSUBX                                            \
if(COND && (BITS(7, 4) == _0011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 63 SBC */
#define EXE_SBC                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = rn - shifter_operand - !CFLAG;                    \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
	else                                                 \
	{                                                    \
      UNPREDICTABLE;                                     \
	}                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
    ASSIGNC(!BorrowFrom(rn, shifter_operand));           \
    ASSIGNV(OverflowFrom_sub(rn, shifter_operand));      \
  }                                                      \
  break;                                                 \
}


/* 64 SEL */
#define EXE_SEL                                                 \
if(COND && (BITS(7, 4) == _1011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 65 SETEND */
#define EXE_SETEND                                                                    \
if( (BITS(31, 28) == _1111_) && (BITS(19, 16) == _0001_) && (BITS(7, 4) == _0000_) )  \
{                                                                                     \
  INSTNOTSUPPORT;                                                                     \
  break;                                                                              \
}

/* 66 SHADD16 */
#define EXE_SHADD16                                             \
if(COND && (BITS(7, 4) == _0001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 67 SHADD8 */
#define EXE_SHADD8                                              \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 68 SHADDSUBX */
#define EXE_SHADDSUBX                                           \
if(COND && (BITS(7, 4) == _0011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 69 SHSUB16 */
#define EXE_SHSUB16                                             \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 70 SHSUB8 */
#define EXE_SHSUB8                                              \
if(COND && (BITS(7, 4) == _1111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 71 SHSUBADDX */
#define EXE_SHSUBADDX                                           \
if(COND && (BITS(7, 4) == _0101_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 72 SMLAXY */
#define EXE_SMLAXY                                              \
if(COND && (BIT(7) == 1) && (BIT(4) == 0) )                     \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 73 SMLAD */
#define EXE_SMLAD                                               \
if(COND && (BITS(7, 6) == _00_) && (BIT(4) == 1) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 74 SMLAL */
#define EXE_SMLAL                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 75 SMLALXY */
#define EXE_SMLALXY                                             \
if(COND && (BIT(7) == 1) && (BIT(4) == 0) )                     \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 76 SMLALD */
#define EXE_SMLALD                                              \
if(COND && (BITS(7, 6) == _00_) && (BIT(4) == 0) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 77 SMLAWY */
#define EXE_SMLAWY                                              \
if(COND && (BIT(7) == 1) && (BITS(5, 4) == _00_) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 78 SMLSD */
#define EXE_SMLSD                                               \
if(COND && (BITS(7, 6) == _01_) && (BIT(4) == 1) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 79 SMLSLD */
#define EXE_SMLSLD                                              \
if(COND && (BITS(7, 6) == _01_) && (BIT(4) == 1) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 80 SMMLA */
#define EXE_SMMLA                                               \
if(COND && (BITS(7, 6) == _00_) && (BIT(4) == 1) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 81 SMMLS */
#define EXE_SMMLS                                               \
if(COND && (BITS(7, 6) == _11_) && (BIT(4) == 1) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 82 SMMUL */
#define EXE_SMMUL                                                                \
if(COND && (BITS(15, 12) == _1111_) && (BITS(7, 6) == _00_) && (BIT(4) == 1) )   \
{                                                                                \
  INSTNOTSUPPORT;                                                                \
  break;                                                                         \
}

/* 83 SMUAD */
#define EXE_SMUAD                                                                \
if(COND && (BITS(15, 12) == _1111_) && (BITS(7, 6) == _00_) && (BIT(4) == 1) )   \
{                                                                                \
  INSTNOTSUPPORT;                                                                \
  break;                                                                         \
}

/* 84 SMULXY */
#define EXE_SMULXY                                              \
if(COND && (BIT(7) == 1) && (BIT(4) == 0) )                     \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 85 SMULL */
#define EXE_SMULL                                                                                   \
if(COND && (BITS(7, 4) == _1001_) )                                                                 \
{                                                                                                   \
  SIGNINT64 result;                                                                                 \
  SIGNINT32 rdhi, rdlo;                                                                             \
  int dhi = BITS(19, 16);                                                                           \
  int dlo = BITS(15, 12);                                                                           \
  int s = BITS(11, 8);                                                                              \
  int m = BITS( 3, 0);                                                                              \
  int sbit = BIT(20);                                                                               \
                                                                                                    \
  result = multiplySigned32((SIGNINT64)(SIGNINT32)Reg(s), (SIGNINT64)(SIGNINT32)Reg(m));            \
  rdhi = (INT32)(result >> 32);                                                                     \
  rdlo = (INT32)(result & 0xffffffff);                                                              \
  writeReg(dhi, rdhi);                                                                              \
  writeReg(dlo, rdlo);                                                                              \
  if(sbit == 1)                                                                                     \
  {                                                                                                 \
    ASSIGNN(GETBIT(rdhi, 31));                                                                      \
    ASSIGNZ( ( (rdhi==0) && (rdlo==0) ) ? 1 : 0 );                                                  \
  }                                                                                                 \
                                                                                                    \
  break;                                                                                            \
}

/* 86 SMULWY */
#define EXE_SMULWY                                              \
if(COND && (BIT(7) == 1) && (BITS(5, 4) == _10_) )              \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 87 SMUSD */
#define EXE_SMUSD                                                                \
if(COND && (BITS(15, 12) == _1111_) && (BITS(7, 6) == _01_) && (BIT(4) == 1) )   \
{                                                                                \
  INSTNOTSUPPORT;                                                                \
  break;                                                                         \
}

/* 88 SRS */
#define EXE_SRS                                                                         \
if( (BITS(31, 28) == _1111_) && (BITS(19, 16) == _1101_) && (BITS(11, 8) == _0101_) )   \
{                                                                                       \
  INSTNOTSUPPORT;                                                                       \
  break;                                                                                \
}

/* 89 SSAT */
#define EXE_SSAT                                                \
if(COND && (BITS(5, 4) == _01_) )                               \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 90 SSAT16 */
#define EXE_SSAT16                                              \
if(COND && (BITS(7, 4) == _0011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 91 SSUB16 */
#define EXE_SSUB16                                              \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 92 SSUB8 */
#define EXE_SSUB8                                               \
if(COND && (BITS(7, 4) == _1111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 93 SSUBADDX */
#define EXE_SSUBADDX                                            \
if(COND && (BITS(7, 4) == _0101_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 94 STC */
#define EXE_STC                                                 \
if( COND )                                                      \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 95 STM1 */
#define EXE_STM1                                 \
if( COND )                                       \
{                                                \
  int i;                                         \
  LOAD_STORE_MULTIPLE;                           \
                                                 \
  INT32 address = start_address;                 \
  for(i=0; i<=15; ++i)                           \
  {                                              \
	if(GETBIT(register_list, i) == 1)            \
	{                                            \
	  writeMemoryWord(address, Reg(i));          \
	  address += 4;                              \
	}                                            \
  }                                              \
  if(end_address != (address - 4))               \
  {                                              \
	PROGRAM_ERROR;                               \
  }                                              \
  break;                                         \
}

/* 96 STM2 */
#define EXE_STM2                                          \
if( COND )                                                \
{                                                         \
  int i;                                                  \
  LOAD_STORE_MULTIPLE;                                    \
                                                          \
  INT32 address = start_address;                          \
  for(i=0; i<=15; ++i)                                    \
  {                                                       \
	if(GETBIT(register_list, i) == 1)                     \
	{                                                     \
	  writeMemoryWord(address, Reg_USR(i));               \
	  address += 4;                                       \
	}                                                     \
  }                                                       \
  if(end_address != (address - 4))                        \
  {                                                       \
	PROGRAM_ERROR;                                        \
  }                                                       \
  break;                                                  \
}

/* 97 STR */
#define EXE_STR                                                \
if( COND )                                                     \
{                                                              \
  LOAD_STORE_WORD_BYTE;                                        \
                                                               \
  INT32 rd = Reg(BITS(15, 12));                                \
  writeMemoryWord(address, rd);                                \
  break;                                                       \
}

/* 98 STRB */
#define EXE_STRB                                               \
if( COND )                                                     \
{                                                              \
  LOAD_STORE_WORD_BYTE;                                        \
                                                               \
  INT32 rd = Reg(BITS(15, 12));                                \
  writeMemoryByte(address, (INT8)(rd & 0xff));                 \
  break;                                                       \
}


/* 99 STRBT */
#define EXE_STRBT                                              \
if( COND )                                                     \
{                                                              \
  LOAD_STORE_WORD_BYTE;                                        \
                                                               \
  INT32 rd = Reg(BITS(15, 12));                                \
  writeMemoryByte(address, (INT8)(rd & 0xff));                 \
  break;                                                       \
}

/* 100 STRD */
#define EXE_STRD                                                \
if(COND && (BITS(7, 4) == _1111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 101 STREX */
#define EXE_STREX                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 102 STRH */
#define EXE_STRH                                               \
if( COND && (BITS(7, 4) == _1011_) )                           \
{                                                              \
  LOAD_STORE_MISCELLANEOUS;                                    \
                                                               \
  INT32 rd = Reg(BITS(15, 12));                                \
  writeMemoryHalfword(address, (INT16)(rd & 0xffff));          \
  break;                                                       \
}

/* 103 STRT */
#define EXE_STRT                                               \
if( COND )                                                     \
{                                                              \
  LOAD_STORE_WORD_BYTE;                                        \
                                                               \
  INT32 rd = Reg(BITS(15, 12));                                \
  writeMemoryWord(address, rd);                                \
  break;                                                       \
}

/* 104 SUB */
#define EXE_SUB                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  rd = rn - shifter_operand;                             \
  writeReg(d, rd);                                       \
  int sbit = BIT(20);                                    \
  if( (sbit == 1) && (d == 15) )                         \
  {                                                      \
    if(CurrentModeHasSPSR())                             \
    {                                                    \
      CPSR_CUR = SPSR_CUR;                               \
    }                                                    \
    else                                                 \
    {                                                    \
      UNPREDICTABLE;                                     \
    }                                                    \
  }                                                      \
  else if(sbit == 1)                                     \
  {                                                      \
    ASSIGNN(GETBIT(rd, 31));                             \
    ASSIGNZ((rd==0) ? 1 : 0);                            \
    ASSIGNC(!BorrowFrom(rn, shifter_operand));           \
	ASSIGNV(OverflowFrom_sub(rn, shifter_operand));      \
  }                                                      \
  break;                                                 \
}

/* 105 SWI */
#define EXE_SWI


/* 106 SWP */
#define EXE_SWP                                                  \
if( COND && (BITS(7, 4) == _1001_) )                             \
{                                                                \
  LOAD_STORE_WORD_BYTE;                                          \
                                                                 \
  INT32 temp = readMemoryWord(address);                          \
  writeMemoryWord(address, Reg(BITS(3, 0)));                     \
  writeReg(BITS(15, 12), temp);                                  \
  break;                                                         \
}


/* 107 SWPB */
#define EXE_SWPB                                                 \
if( COND && (BITS(7, 4) == _1001_) )                             \
{                                                                \
  LOAD_STORE_WORD_BYTE;                                          \
                                                                 \
  INT8 temp = readMemoryByte(address);                           \
  writeMemoryByte(address, (INT8)(Reg(BITS(3, 0)) & 0xff));      \
  writeReg(BITS(15, 12), temp);                                  \
  break;                                                         \
}

/* 108 SXTAB */
#define EXE_SXTAB                                               \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 109 SXTAB16 */
#define EXE_SXTAB16                                             \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 110 SXTAH */
#define EXE_SXTAH                                               \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 111 SXTB */
#define EXE_SXTB                                                   \
if(COND && (BITS(19, 16) == _1111_) && (BITS(7, 4) == _0111_) )    \
{                                                                  \
  INSTNOTSUPPORT;                                                  \
  break;                                                           \
}

/* 112 SXTB16 */
#define EXE_SXTB16                                                 \
if(COND && (BITS(19, 16) == _1111_) && (BITS(7, 4) == _0111_) )    \
{                                                                  \
  INSTNOTSUPPORT;                                                  \
  break;                                                           \
}

/* 113 SXTH */
#define EXE_SXTH                                                   \
if(COND && (BITS(19, 16) == _1111_) && (BITS(7, 4) == _0111_) )    \
{                                                                  \
  INSTNOTSUPPORT;                                                  \
  break;                                                           \
}

/* 114 TEQ */
#define EXE_TEQ                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  INT32 alu_out = rn ^ shifter_operand;                  \
  ASSIGNN(GETBIT(alu_out, 31));                          \
  ASSIGNZ((alu_out==0) ? 1 : 0);                         \
  ASSIGNC(shifter_carry_out);                            \
  break;                                                 \
}

/* 115 TST */
#define EXE_TST                                          \
if(COND)                                                 \
{                                                        \
  DATA_PROCESSING_OPERANDS;                              \
  INT32 alu_out = rn & shifter_operand;                  \
  ASSIGNN(GETBIT(alu_out, 31));                          \
  ASSIGNZ((alu_out==0) ? 1 : 0);                         \
  ASSIGNC(shifter_carry_out);                            \
  break;                                                 \
}

/* 116 UADD16 */
#define EXE_UADD16                                              \
if(COND && (BITS(7, 4) == _0001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 117 UADD8 */
#define EXE_UADD8                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 118 UADDSUBX */
#define EXE_UADDSUBX                                            \
if(COND && (BITS(7, 4) == _0011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 119 UHADD16 */
#define EXE_UHADD16                                             \
if(COND && (BITS(7, 4) == _0001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 120 UHADD8 */
#define EXE_UHADD8                                              \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 121 UHADDSUBX */
#define EXE_UHADDSUBX                                           \
if(COND && (BITS(7, 4) == _0011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 122 UHSUB16 */
#define EXE_UHSUB16                                             \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 123 UHSUB8 */
#define EXE_UHSUB8                                              \
if(COND && (BITS(7, 4) == _1111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 124 UHSUBADDX */
#define EXE_UHSUBADDX                                           \
if(COND && (BITS(7, 4) == _0101_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 125 UMAAL */
#define EXE_UMAAL                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 126 UMLAL */
#define EXE_UMLAL                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INT64 result;                                                 \
  int dhi = BITS(19, 16);                                       \
  int dlo = BITS(15, 12);                                       \
  int s = BITS(11, 8);                                          \
  int m = BITS( 3, 0);                                          \
  int sbit = BIT(20);                                           \
  INT32 rdhi = Reg(dhi);                                        \
  INT32 rdlo = Reg(dlo);                                        \
  INT32 high, low;                                              \
                                                                \
  result = multiplyUnsigned32((INT64)Reg(s), (INT64)Reg(m));    \
  high = (INT32)(result >> 32);                                 \
  low  = (INT32)(result & 0xffffffff);                          \
                                                                \
  /* first rdhi then rdlo */                                    \
  rdhi += ( high + CarryFrom(low, rdlo) );                      \
  rdlo += low;                                                  \
                                                                \
  writeReg(dhi, rdhi);                                          \
  writeReg(dlo, rdlo);                                          \
                                                                \
  if(sbit)                                                      \
  {                                                             \
    ASSIGNN(GETBIT(rdhi, 31));                                  \
    ASSIGNZ( ( (rdhi==0) && (rdlo==0) ) ? 1 : 0 );              \
  }                                                             \
                                                                \
  break;                                                        \
}

/* 127 UMULL */
#define EXE_UMULL                                               \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INT64 result;                                                 \
  INT32 rdhi, rdlo;                                             \
  int dhi = BITS(19, 16);                                       \
  int dlo = BITS(15, 12);                                       \
  int s = BITS(11, 8);                                          \
  int m = BITS( 3, 0);                                          \
  int sbit = BIT(20);                                           \
                                                                \
  result = multiplyUnsigned32((INT64)Reg(s), (INT64)Reg(m));    \
  rdhi = (INT32)(result >> 32);                                 \
  rdlo = (INT32)(result & 0xffffffff);                          \
  writeReg(dhi, rdhi);                                          \
  writeReg(dlo, rdlo);                                          \
  if(sbit == 1)                                                 \
  {                                                             \
    ASSIGNN(GETBIT(rdhi, 31));                                  \
    ASSIGNZ( ( (rdhi==0) && (rdlo==0) ) ? 1 : 0 );              \
  }                                                             \
                                                                \
  break;                                                        \
}

/* 128 UQADD16 */
#define EXE_UQADD16                                             \
if(COND && (BITS(7, 4) == _0001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 129 UQADD8 */
#define EXE_UQADD8                                              \
if(COND && (BITS(7, 4) == _1001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 130 UQADDSUBX */
#define EXE_UQADDSUBX                                           \
if(COND && (BITS(7, 4) == _0011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 131 UQSUB16 */
#define EXE_UQSUB16                                             \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 132 UQSUB8 */
#define EXE_UQSUB8                                              \
if(COND && (BITS(7, 4) == _1111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 133 UQSUBADDX */
#define EXE_UQSUBADDX                                           \
if(COND && (BITS(7, 4) == _0101_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 134 USAD8 */
#define EXE_USAD8                                               \
if(COND && (BITS(7, 4) == _0001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}


/* 135 USADA8 */
#define EXE_USADA8                                              \
if(COND && (BITS(7, 4) == _0001_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 136 USAT */
#define EXE_USAT                                                \
if(COND && (BITS(5, 4) == _01_) )                               \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 137 USAT16 */
#define EXE_USAT16                                              \
if(COND && (BITS(7, 4) == _0011_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 138 USUB16 */
#define EXE_USUB16                                              \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 139 USUB8 */
#define EXE_USUB8                                               \
if(COND && (BITS(7, 4) == _1111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 140 USUBADDX */
#define EXE_USUBADDX                                            \
if(COND && (BITS(7, 4) == _0101_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 141 UXTAB */
#define EXE_UXTAB                                               \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 142 UXTAB16 */
#define EXE_UXTAB16                                             \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 143 UXTAH */
#define EXE_UXTAH                                               \
if(COND && (BITS(7, 4) == _0111_) )                             \
{                                                               \
  INSTNOTSUPPORT;                                               \
  break;                                                        \
}

/* 144 UXTB */
#define EXE_UXTB                                                  \
if(COND && (BITS(19, 16) == _1111_) && (BITS(7, 4) == _0111_) )   \
{                                                                 \
  INSTNOTSUPPORT;                                                 \
  break;                                                          \
}

/* 145 UXTB16 */
#define EXE_UXTB16                                                \
if(COND && (BITS(19, 16) == _1111_) && (BITS(7, 4) == _0111_) )   \
{                                                                 \
  INSTNOTSUPPORT;                                                 \
  break;                                                          \
}



/* 146 UXTH */
#define EXE_UXTH                                                  \
if(COND && (BITS(19, 16) == _1111_) && (BITS(7, 4) == _0111_) )   \
{                                                                 \
  INSTNOTSUPPORT;                                                 \
  break;                                                          \
}


void initInstruction();
int execIns(INT32 start);

#endif
