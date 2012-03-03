
#ifndef ELF_H_
#define ELF_H_

#include <stdio.h>
#include "common.h"



INT32 getFileEntryPoint(char * fileStart);
INT32 getEntryPointVa(char * buffer);
INT32 getFileLength(FILE * fp);


#endif
