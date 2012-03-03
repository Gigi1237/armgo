#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cpu.h"
#include "memory.h"



CPU cpu;


INT32 * regTable[18][7] =
   /* User */ /* System */ /* Supervisor */ /* Abort */ /* Undefined */ /* Interrupt */ /* Fast interrupt */
{ { &cpu.r[0],  &cpu.r[0],     &cpu.r[0],     &cpu.r[0],    &cpu.r[0],     &cpu.r[0],         &cpu.r[0]  },
  { &cpu.r[1],  &cpu.r[1],     &cpu.r[1],     &cpu.r[1],    &cpu.r[1],     &cpu.r[1],         &cpu.r[1]  },
  { &cpu.r[2],  &cpu.r[2],     &cpu.r[2],     &cpu.r[2],    &cpu.r[2],     &cpu.r[2],         &cpu.r[2]  },
  { &cpu.r[3],  &cpu.r[3],     &cpu.r[3],     &cpu.r[3],    &cpu.r[3],     &cpu.r[3],         &cpu.r[3]  },
  { &cpu.r[4],  &cpu.r[4],     &cpu.r[4],     &cpu.r[4],    &cpu.r[4],     &cpu.r[4],         &cpu.r[4]  },
  { &cpu.r[5],  &cpu.r[5],     &cpu.r[5],     &cpu.r[5],    &cpu.r[5],     &cpu.r[5],         &cpu.r[5]  },
  { &cpu.r[6],  &cpu.r[6],     &cpu.r[6],     &cpu.r[6],    &cpu.r[6],     &cpu.r[6],         &cpu.r[6]  },
  { &cpu.r[7],  &cpu.r[7],     &cpu.r[7],     &cpu.r[7],    &cpu.r[7],     &cpu.r[7],         &cpu.r[7]  },
  { &cpu.r[8],  &cpu.r[8],     &cpu.r[8],     &cpu.r[8],    &cpu.r[8],     &cpu.r[8],         &cpu.r[24] },
  { &cpu.r[9],  &cpu.r[9],     &cpu.r[9],     &cpu.r[9],    &cpu.r[9],     &cpu.r[9],         &cpu.r[25] },
  { &cpu.r[10], &cpu.r[10],    &cpu.r[10],    &cpu.r[10],   &cpu.r[10],    &cpu.r[10],        &cpu.r[26] },
  { &cpu.r[11], &cpu.r[11],    &cpu.r[11],    &cpu.r[11],   &cpu.r[11],    &cpu.r[11],        &cpu.r[27] },
  { &cpu.r[12], &cpu.r[12],    &cpu.r[12],    &cpu.r[12],   &cpu.r[12],    &cpu.r[12],        &cpu.r[28] },
  { &cpu.r[13], &cpu.r[13],    &cpu.r[16],    &cpu.r[18],   &cpu.r[20],    &cpu.r[22],        &cpu.r[29] },
  { &cpu.r[14], &cpu.r[14],    &cpu.r[17],    &cpu.r[19],   &cpu.r[21],    &cpu.r[23],        &cpu.r[30] },
  { &cpu.r[15], &cpu.r[15],    &cpu.r[15],    &cpu.r[15],   &cpu.r[15],    &cpu.r[15],        &cpu.r[15] },
  { &cpu.r[31], &cpu.r[31],    &cpu.r[31],    &cpu.r[31],   &cpu.r[31],    &cpu.r[31],        &cpu.r[31] },
  { NULL,       NULL,          &cpu.r[32],    &cpu.r[33],   &cpu.r[34],    &cpu.r[35],        &cpu.r[36] } };




#define FETCH16(index)                                         \
{                                                              \
  cpu.pipeline16[index].ins = readMemoryHalfword(PC);          \
  cpu.pipeline16[index].address = (INT32)(PC);                 \
  PC += 2;                                                     \
}


#define FETCH32(index)                                         \
{                                                              \
  cpu.pipeline32[index].ins = readMemoryWord(PC);              \
  cpu.pipeline32[index].address = (INT32)(PC);                 \
  PC += 4;                                                     \
}




// refresh instruction pipeline(current pc has already been new)
void refreshPipeline()
{
  if(TFLAG)
  {
    FETCH16(0);
    FETCH16(1);
  }
  else
  {
    FETCH32(0);
    FETCH32(1);
  }
  cpu.pipelineRefreshed = 0;
}



void nextPipeline()
{
  if(TFLAG)
  {
    cpu.pipeline16[0] = cpu.pipeline16[1];
    FETCH16(1);
  }
  else
  {
    cpu.pipeline32[0] = cpu.pipeline32[1];
    FETCH32(1);
  }
}







inline int InAPrivilegedMode()
{
  if( cpu.mode == MODE_USR)
    return 0;
  else
    return 1;
}







void writeReg(int index, INT32 value)
{
  if(index == 15)
  {
	if(TFLAG)
  	{
	  Reg(index) = ( value & 0xfffffffe );
  	}
	else
	{
      Reg(index) = ( value & 0xfffffffc );
	}
	// instruction pipeline should be refreshed
	cpu.pipelineRefreshed = 1;
  }
  else if(index>=0 && index <=14)
  {
	Reg(index) = ( value & 0xffffffff );
  }
  else
  {
	PROGRAM_ERROR;
  }
}









int arrayHexToDigit(char * s)
{
    int i,temp=0,n;
    int size = strlen(s);
    for(i=0;i<size;i++)
    {
        if(s[i]>='A'&&s[i]<='F')//十六进制还要判断他是不是在A-F或者a-f之间a=10。。
         n=s[i]-'A'+10;
        else if(s[i]>='a'&&s[i]<='f')
         n=s[i]-'a'+10;
         else n=s[i]-'0';
        temp=temp*16+n;
    }
    return temp;
}








void initCPU()
{
  TFLAG = 0;
}



INT32 getCP15(int index)
{
  if( (index<0) || (index>15) )
  {
    PROGRAM_ERROR;
    return 0;
  }

  return cpu.coprocessor[15][index];
}


void setCP15(int index, INT32 value)
{
  if( (index<0) || (index>15) )
  {
    PROGRAM_ERROR;
  }

  if(index == 1)
  {
    printf("CP15(1) can not be written!\n");
  }
  else
  {
    cpu.coprocessor[15][index] = value;
  }
}







int bp;
int inscount;
int sss[256];
int scount;

INT32 rrr(INT32 address)
{
  return readMemoryWord(address);
}


void dumpCpu()
{
  inscount++;
  //printf("ins count = %d\n", inscount);;
  //if((cpu.pipeline32[0].address == 0x33f8ae40) || (bbb==1) )

  if( (bp !=0 ) && ( inscount >= bp ) )
  {
    printf("ins count = %d\n", inscount);
    printf("r0  = 0x%08x, r1  = 0x%08x, r2  = 0x%08x, r3  = 0x%08x\n", Reg(0),  Reg(1),  Reg(2),  Reg(3));
    printf("r4  = 0x%08x, r5  = 0x%08x, r6  = 0x%08x, r7  = 0x%08x\n", Reg(4),  Reg(5),  Reg(6),  Reg(7));
    printf("r8  = 0x%08x, r9  = 0x%08x, r10 = 0x%08x, r11 = 0x%08x\n", Reg(8),  Reg(9),  Reg(10), Reg(11));
    printf("r12 = 0x%08x, r13 = 0x%08x, r14 = 0x%08x, r15 = 0x%08x\n", Reg(12), Reg(13), Reg(14), Reg(15));
    printf("\n");


  char str[100];
  while(1)
  {
    fflush(stdin);
    gets(str);

      if(str[0] != 'm')
        break;


      INT32 address = arrayHexToDigit(str+1);

    printf("[0x%08x] = 0x%08x\n", address + 0x00, readMemoryWord(address + 0x00));
    printf("[0x%08x] = 0x%08x\n", address + 0x04, readMemoryWord(address + 0x04));
    printf("[0x%08x] = 0x%08x\n", address + 0x08, readMemoryWord(address + 0x08));
    printf("[0x%08x] = 0x%08x\n", address + 0x0c, readMemoryWord(address + 0x0c));
    printf("[0x%08x] = 0x%08x\n", address + 0x10, readMemoryWord(address + 0x10));
    printf("[0x%08x] = 0x%08x\n", address + 0x14, readMemoryWord(address + 0x14));
    printf("[0x%08x] = 0x%08x\n", address + 0x18, readMemoryWord(address + 0x18));
    printf("[0x%08x] = 0x%08x\n", address + 0x1c, readMemoryWord(address + 0x1c));
  }
  }
}






int startSim(INT32 start)
{
  //printf("Break Point: 0x");
  //scanf("%x", &bp);
  //printf("Ins count: ");
  //scanf("%d", &bp);

  initInstruction();

  writeReg(15, start);

  while(1)
  {
	//usleep(1);
	//if( (bp !=0 ) && ( inscount >= bp ) )
	//{
	//}
	/*
	  if( (bp!=0) || (PC == 0x33f8ce04) )
	  {
		  fflush(stdin);
		    printf("r0  = 0x%08x, r1  = 0x%08x, r2  = 0x%08x, r3  = 0x%08x\n", Reg(0),  Reg(1),  Reg(2),  Reg(3));
		    printf("r4  = 0x%08x, r5  = 0x%08x, r6  = 0x%08x, r7  = 0x%08x\n", Reg(4),  Reg(5),  Reg(6),  Reg(7));
		    printf("r8  = 0x%08x, r9  = 0x%08x, r10 = 0x%08x, r11 = 0x%08x\n", Reg(8),  Reg(9),  Reg(10), Reg(11));
		    printf("r12 = 0x%08x, r13 = 0x%08x, r14 = 0x%08x, r15 = 0x%08x\n", Reg(12), Reg(13), Reg(14), Reg(15));
			printf("At 0x%08x: %08x\n", (cpu.pipeline32[0].address), cpu.pipeline32[0].ins);
		    printf("\n");
		  getchar();
		  bp = 1;
	  }
*/
/*
	  printf("At 0x%08x: %08x\n", (cpu.pipeline32[0].address), cpu.pipeline32[0].ins);
      printf("r0  = 0x%08x, r1  = 0x%08x, r2  = 0x%08x, r3  = 0x%08x\n", Reg(0),  Reg(1),  Reg(2),  Reg(3));
	  printf("r4  = 0x%08x, r5  = 0x%08x, r6  = 0x%08x, r7  = 0x%08x\n", Reg(4),  Reg(5),  Reg(6),  Reg(7));
	  printf("r8  = 0x%08x, r9  = 0x%08x, r10 = 0x%08x, r11 = 0x%08x\n", Reg(8),  Reg(9),  Reg(10), Reg(11));
	  printf("r12 = 0x%08x, r13 = 0x%08x, r14 = 0x%08x, r15 = 0x%08x\n", Reg(12), Reg(13), Reg(14), Reg(15));
	  printf("CPSR = 0x%08x\n", CPSR_CUR);
	  printf("\n");
	  fflush(stdin);
	  getchar();
*/

	if(cpu.pipelineRefreshed)
	{
	  refreshPipeline();
	}
	else
	{
	  nextPipeline();
	}

/*
    printf("r0  = 0x%08x, r1  = 0x%08x, r2  = 0x%08x, r3  = 0x%08x\n", Reg(0),  Reg(1),  Reg(2),  Reg(3));
    printf("r4  = 0x%08x, r5  = 0x%08x, r6  = 0x%08x, r7  = 0x%08x\n", Reg(4),  Reg(5),  Reg(6),  Reg(7));
    printf("r8  = 0x%08x, r9  = 0x%08x, r10 = 0x%08x, r11 = 0x%08x\n", Reg(8),  Reg(9),  Reg(10), Reg(11));
    printf("r12 = 0x%08x, r13 = 0x%08x, r14 = 0x%08x, r15 = 0x%08x\n", Reg(12), Reg(13), Reg(14), Reg(15));
	printf("At 0x%08x: %08x\n", (cpu.pipeline32[0].address), cpu.pipeline32[0].ins);
    printf("\n");
	getchar();
*/

    execIns(start);


    dumpCpu();
  }
}

