#include "rtos.h"
#include "glib.h"
#include "systick.h"
#include "leds.h"
#include "timer.h"
#include "mem.h"


void task_init(void);

void func_task01(void)
{
	int i = 0;
	while(1) {
		printf("t 01 i=%d\n\r", i++);
	}
}

void func_task02(void)
{
        int i = 0;
        while(1) {
                printf("t 02 i=%d\n\r", i++);
        }
}

void func_task03(void)
{
        int i = 0;
        while(1) {
                printf("t 03 i=%d\n\r", i++);
        }
}

void rtos_start(void)
{
	task_init();
	systick_init(71999);
}

typedef void(*ins_ptr)(void);

struct task_init_stack_frame {
    u32 r4_to_r11[8];       // lower address
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r12;
    u32 lr;
    ins_ptr pc;
    u32 xpsr;               // higher address
};

struct task_p {
    u32 stack[256];
    struct task_init_stack_frame base;
    void *basep;
};

struct task_list {
    struct task_p *task;
    struct task_list *next;
};

struct task_p task01;
struct task_p task02;
struct task_p task03;

void* next_stack;
struct task_list head;
struct task_list *run;

void task_init(void)
{
	struct task_list *p;
	task01.base.r0 = 0;
	task01.base.pc = (ins_ptr)(&func_task01);
	task01.base.lr = 0;
	task01.base.xpsr = 0x61000000;
	head.task = &task01;
        head.next = &head;
        task01.basep = &(task01.base);

        task02.base.r0 = 0;
        task02.base.pc = (ins_ptr)(&func_task02);
        task02.base.lr = 0;
        task02.base.xpsr = 0x61000000;
	p = malloc(sizeof(struct task_list));
	p->next = head.next;
	head.next = p;
	p->task = &task02;
    	task02.basep = &(task02.base);

        task03.base.r0 = 0;
        task03.base.pc = (ins_ptr)(&func_task03);
        task03.base.lr = 0;
        task03.base.xpsr = 0x61000000;
	p = malloc(sizeof(struct task_list));
        p->next = head.next;
        head.next = p;
        p->task = &task03;
	task03.basep = &(task03.base);

	run = &head;
}



void* tick_and_switch(void* cur_stack)
{
    void* temp ;
/*
	temp = next_stack;
    next_stack = cur_stack;
*/
    run->task->basep = cur_stack;
    run = run->next;
    temp = run->task->basep;
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

