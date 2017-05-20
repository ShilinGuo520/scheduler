#include "mem.h"
#include "nvic.h"

#define MEM_BASE    0x2000a000
#define MEM_SIZE    0x00005000  //20K
#define BLOCK_SIZE  0x1         //byt

#define MAP_SIZE    (MEM_SIZE/BLOCK_SIZE/8)

unsigned char mem_map[MAP_SIZE];

void * malloc(int size)
{
    DISABLE_INTERRUPT();
    void * ret = NULL;
    int find = 0;
    int i,j;
    if(size <= 0)
        return ret;

    size = size + BLOCK_SIZE - 1;

    for (i = 0; i < (MEM_SIZE); i++) {
        for (j = 0; j < 8; j++) {
            if (mem_map[i] & (1 << j)) {
               ret = NULL; 
               find = 0;
               continue;
            } else {
                if (find == 0)
                    ret = (void *)(long)(MEM_BASE + ((i * 8) + j) * BLOCK_SIZE);
                find = find + BLOCK_SIZE;

                if (find >= size) {
                    j++;
                    if(j >= 8) {
                        j = 0;
                        i++;
                    }
                    while (find > 0) {
                        mem_map[i] |= (1 << j--);
                        find = find - BLOCK_SIZE;
                        if (j < 0) {
                            j = 7;
                            i--;
                        }
                    }
                    return ret;
                }
            }
        }
    }
    return NULL;
}

int free(void *mem)
{
    long mem_add = (long)mem;
    int i = (mem_add - MEM_BASE)/BLOCK_SIZE/8;
    int j = (((mem_add - MEM_BASE)/BLOCK_SIZE)%8) + 1;
    if(j >= 8) {
        i++;
        j = 0;
    }
    while ((mem_map[i] & (1 << j)) && ( i < MAP_SIZE )) {
        mem_map[i] &= ~(1 << j);
        j++;
        if(j >= 8) {
            i++;
            j = 0;
        }
    }
}

