#ifndef _MEM_H_
#define _MEM_H_

#include "types.h"

extern void * malloc(int size);
extern int free(void *mem);
extern void memcpy(unsigned char *dec, unsigned char *src, int size);                                                            
extern void memset(unsigned char *src, unsigned char val, int size);

#endif
