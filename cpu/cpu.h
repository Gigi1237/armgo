
#ifndef CPU_H_
#define CPU_H_


#include "common.h"
#include "boot.h"



typedef struct
{
  INT32 ins;
  INT32 address;
}PIPELINE32;

typedef struct
{
  INT16 ins;
  INT32 address;
}PIPELINE16;


typedef enum
{
  MODE_USR = 0,
  MODE_SYS = 1,
  MODE_SVC = 2,
  MODE_ABT = 3,
  MODE_UND = 4,
  MODE_IRQ = 5,
  MODE_FIQ = 6
}CPUMODE;


#define MODE_USER_CODE        0x10
#define MODE_FIQ_CODE         0x11
#define MODE_IRQ_CODE         0x12
#define MODE_SUPERVISOR_CODE  0x13
#define MODE_ABORT_CODE       0x17
#define MODE_UNDEFINED_CODE   0x1b
#define MODE_SYSTEM_CODE      0x1f




typedef struct CPU
{
  /*  0  r0       */
  /*  1  r1       */
  /*  2  r2       */
  /*  3  r3       */
  /*  4  r4       */
  /*  5  r5       */
  /*  6  r6       */
  /*  7  r7       */
  /*  8  r8       */
  /*  9  r9       */
  /* 10  r10      */
  /* 11  r11      */
  /* 12  r12      */
  /* 13  r13(sp)  */
  /* 14  r14(lr)  */
  /* 15  r15(pc)  */
  /* 16  r13_svc  */
  /* 17  r14_svc  */
  /* 18  r13_abt  */
  /* 19  r14_abt  */
  /* 20  r13_und  */
  /* 21  r14_und  */
  /* 22  r13_irq  */
  /* 23  r14_irq  */
  /* 24  r8_fiq   */
  /* 25  r9_fiq   */
  /* 26  r10_fiq  */
  /* 27  r11_fiq  */
  /* 28  r12_fiq  */
  /* 29  r13_fiq  */
  /* 30  r14_fiq  */
  /* 31  cpsr     */
  /* 32  spsr_svc */
  /* 33  spsr_abt */
  /* 34  spsr_und */
  /* 35  spsr_irq */
  /* 36  spsr_fiq */
  INT32 r[40];

  INT32 coprocessor[16][16];

  INT32 zflag;
  INT32 nflag;
  INT32 vflag;
  INT32 cflag;
  INT32 tflag;
  INT32 qflag;

  /* current mode */
  CPUMODE mode;

  // instruction pipeline
  int pipelineRefreshed;
  PIPELINE32 pipeline32[2];
  PIPELINE16 pipeline16[2];
}CPU;


extern CPU cpu;
extern INT32 * regTable[18][7];



// coprocessor
#define CP15(n)           (cpu.coprocessor[15][n])
#define Coprocessor(i, j) (cpu.coprocessor[i][j])


// current mode
#define CURMODE (cpu.mode)
#define SP_CUR   (*regTable[13][cpu.mode])
#define LR_CUR   (*regTable[14][cpu.mode])
#define PC       (*regTable[15][cpu.mode])
#define CPSR_CUR (*regTable[16][cpu.mode])
#define SPSR_CUR (*regTable[17][cpu.mode])
#define Reg(i) (*regTable[i][cpu.mode])

#define Reg_USR(i) (*regTable[i][MODE_USR])
#define Reg_SYS(i) (*regTable[i][MODE_SYS])
#define Reg_SVC(i) (*regTable[i][MODE_SVC])
#define Reg_ABT(i) (*regTable[i][MODE_ABT])
#define Reg_UND(i) (*regTable[i][MODE_UND])
#define Reg_IRQ(i) (*regTable[i][MODE_IRQ])
#define Reg_FIQ(i) (*regTable[i][MODE_FIQ])

#define CPSR_USR (*regTable[16][MODE_USR])
#define CPSR_SYS (*regTable[16][MODE_SYS])
#define CPSR_SVC (*regTable[16][MODE_SVC])
#define CPSR_ABT (*regTable[16][MODE_ABT])
#define CPSR_UND (*regTable[16][MODE_UND])
#define CPSR_IRQ (*regTable[16][MODE_IRQ])
#define CPSR_FIQ (*regTable[16][MODE_FIQ])

#define SPSR_USR (*regTable[17][MODE_USR])
#define SPSR_SYS (*regTable[17][MODE_SYS])
#define SPSR_SVC (*regTable[17][MODE_SVC])
#define SPSR_ABT (*regTable[17][MODE_ABT])
#define SPSR_UND (*regTable[17][MODE_UND])
#define SPSR_IRQ (*regTable[17][MODE_IRQ])
#define SPSR_FIQ (*regTable[17][MODE_FIQ])


#define MODEREG(i, mode) (*regTable[i][mode])



// flags operation
#define SETZ (cpu.zflag=1)
#define SETC (cpu.cflag=1)
#define SETV (cpu.vflag=1)
#define SETN (cpu.nflag=1)
#define SETT (cpu.tflag=1)
#define SETQ (cpu.qflag=1)

#define CLEARZ (cpu.zflag=0)
#define CLEARC (cpu.cflag=0)
#define CLEARV (cpu.vflag=0)
#define CLEARN (cpu.nflag=0)
#define CLEART (cpu.tflag=0)
#define CLEARQ (cpu.qflag=0)

#define ASSIGNZ(b) (cpu.zflag = b)
#define ASSIGNC(b) (cpu.cflag = b)
#define ASSIGNV(b) (cpu.vflag = b)
#define ASSIGNN(b) (cpu.nflag = b)
#define ASSIGNT(b) (cpu.tflag = b)
#define ASSIGNQ(b) (cpu.qflag = b)


#define ZFLAG (cpu.zflag)
#define CFLAG (cpu.cflag)
#define VFLAG (cpu.vflag)
#define NFLAG (cpu.nflag)
#define TFLAG (cpu.tflag)
#define QFLAG (cpu.qflag)


#define TOPBITS(n)      ( TFLAG ? ( ( cpu.pipeline16[0].ins ) >> (n) ) : ( ( cpu.pipeline32[0].ins ) >> (n) ) )
#define BITS(high, low) ( ( ( cpu.pipeline32[0].ins ) >> (low) ) & andMask[ (high) - (low) ] )
#define BIT(n)          ( TFLAG ? ( ( ( cpu.pipeline16[0].ins ) >> (n) ) & 1) : ( ( ( cpu.pipeline32[0].ins ) >> (n) ) & 1) )




#define get_CP15_reg1_L2bit ( GETBIT(CP15(1), 26) )
#define get_CP15_reg1_EEbit ( GETBIT(CP15(1), 25) )
#define get_CP15_reg1_VEbit ( GETBIT(CP15(1), 24) )
#define get_CP15_reg1_XPbit ( GETBIT(CP15(1), 23) )
#define get_CP15_reg1_Ubit  ( GETBIT(CP15(1), 22) )
#define get_CP15_reg1_FIbit ( GETBIT(CP15(1), 21) )
#define get_CP15_reg1_L4bit ( GETBIT(CP15(1), 15) )
#define get_CP15_reg1_RRbit ( GETBIT(CP15(1), 14) )
#define get_CP15_reg1_Vbit  ( GETBIT(CP15(1), 13) )
#define get_CP15_reg1_Ibit  ( GETBIT(CP15(1), 12) )
#define get_CP15_reg1_Zbit  ( GETBIT(CP15(1), 11) )
#define get_CP15_reg1_Fbit  ( GETBIT(CP15(1), 10) )
#define get_CP15_reg1_Rbit  ( GETBIT(CP15(1),  9) )
#define get_CP15_reg1_Sbit  ( GETBIT(CP15(1),  8) )
#define get_CP15_reg1_Bbit  ( GETBIT(CP15(1),  7) )
#define get_CP15_reg1_Lbit  ( GETBIT(CP15(1),  6) )
#define get_CP15_reg1_Dbit  ( GETBIT(CP15(1),  5) )
#define get_CP15_reg1_Pbit  ( GETBIT(CP15(1),  4) )
#define get_CP15_reg1_Wbit  ( GETBIT(CP15(1),  3) )
#define get_CP15_reg1_Cbit  ( GETBIT(CP15(1),  2) )
#define get_CP15_reg1_Abit  ( GETBIT(CP15(1),  1) )
#define get_CP15_reg1_Mbit  ( GETBIT(CP15(1),  0) )



void initCPU();
int startSim(INT32 start);
extern int execIns(INT32 start);
INT32 getCP15(int index);
void setCP15(int index, INT32 value);
void writeReg(int index, INT32 value);


#endif



