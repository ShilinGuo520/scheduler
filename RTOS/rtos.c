#include "rtos.h"
#include "glib.h"
#include "systick.h"
#include "leds.h"
#include "timer.h"
#include "mem.h"
#include "types.h"

#define     IDE_TASK_STACK      256

#define     TASK_STATUS_DONE    1
struct task_pcb {
    unsigned char status;
    unsigned long *task_stack;
};

struct task_list_p {
    struct task_pcb task;
    struct task_list_p *next;
};

struct task_list_p *task_head;
struct task_list_p *task_run;

int creat_task(void (*func), int stack_size)
{
    unsigned long *stack;
    struct task_list_p *task = malloc(sizeof(struct task_list_p));
    task->next = task_head->next;
    task_head->next = task;

    stack = malloc(stack_size);
    if(stack == NULL)
        return -1;

    stack = stack + ((stack_size/4)*4);
    task->task.task_stack = stack;

    *stack-- = 0x61000000;
    *stack-- = (unsigned long)(func);

    memset((unsigned char *)stack, 0, 14 * 4);

    task->task.status = TASK_STATUS_DONE;
}

void ide_task(void);

void task_init(void)
{
    unsigned long *stack;
    task_head = malloc(sizeof(struct task_list_p));
    task_head->next = task_head;
    stack = malloc(IDE_TASK_STACK);
    
    stack = stack + ((IDE_TASK_STACK/4)*4);
    task_head->task.task_stack = stack;

    *stack-- = 0x61000000;
    *stack-- = (unsigned long)(ide_task);

    memset((unsigned char *)stack, 0, 14 * 4) ;

    task_run = task_head;

    task_head->task.status = TASK_STATUS_DONE;
}

void ide_task(void)
{
	int i = 0;
	while(1) {
		printf("ide i=%d\n\r", i++);
	}
}


void rtos_start(void)
{
	task_init();
	systick_init(71999);
}


void* tick_and_switch(void* cur_stack)
{
    void *temp;
    task_run->task.task_stack = cur_stack;
    task_run = task_run->next;
    temp = task_run->task.task_stack;
    return temp;
}


void SysTick_Handler()
{
__asm volatile (
"    mov r0, sp          	\n"
"    sub r0, #(8*4)      	\n"
"    push {lr}				\n"
"    bl.w tick_and_switch 	\n"
"    pop {lr}				\n"
"							\n"
"    cmp r0,#0   			\n"
"    beq end				\n"
"							\n"
"    push {r4-r11}   		\n"
"    mov sp,r0       		\n"
"    pop  {r4-r11}   		\n"
"							\n"
"end:             			\n"
	);
}

