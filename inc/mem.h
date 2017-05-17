#ifndef _MEM_H_
#define _MEM_H_

#include "types.h"

struct list {
    char data;
    struct list *next; 
};

void * malloc(int size);
int free(void *mem);

struct list *malloc_list(int size);
int free_list(struct list *head);

#endif
