#include "rtos.h"
#include "glib.h"
#include "systick.h"
#include "leds.h"
#include "timer.h"
#include "mem.h"


#define	IDLE_TASK_STACK_SIZE		256

typedef void(*ins_ptr)(void);

void idle_task(void)
{
	int i = 0;
	while(1) {
		printf("idle task i=%d\n\r", i++);
	}
}

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
    unsigned int status;
    struct task_init_stack_frame *base;
    unsigned char *stack;
    void *basep;
};

struct task_list {
    struct task_p *task;
    struct task_list *next;
};


struct task_list head;
struct task_list *run;

void os_delay_ms(int time)
{
	//TODO
}

void task_init(void)
{
	struct task_list *p;
	struct task_p *task;
	
	task = malloc(sizeof(struct task_p));
	task->stack = malloc(IDLE_TASK_STACK_SIZE);
	task->base = malloc(sizeof(struct task_init_stack_frame));

	task->base->r0 = 0;
	task->base->pc = (ins_ptr)(&idle_task);
	task->base->lr = 0;
	task->base->xpsr = 0x61000000;
	task->basep = task->base;

    portDISABLE_INTERRUPTS();
	head.task = task;
    	head.next = &head;
	run = &head;
    portENABLE_INTERRUPTS();	

	task->status = 0x0000;
}

int creat_task(void (*func), int stack_size)
{
    struct task_list *p;
    struct task_p *task;

    task = malloc(sizeof(struct task_p));
    task->stack = malloc(stack_size);
    task->base = malloc(sizeof(struct task_init_stack_frame));
    task->base->r0 = 0;
    task->base->pc = (ins_ptr)(func);
    task->base->lr = 0;
    task->base->xpsr = 0x61000000;
    task->basep = task->base;

    p = malloc(sizeof(struct task_list));

    portDISABLE_INTERRUPTS();
    p->next = head.next;
    head.next = p;
    p->task = task;
    portENABLE_INTERRUPTS();

    task->status = 0x0000;
}

void rtos_start(void)
{
    systick_init(71999);

__asm volatile (
	"ldr r0,=0x2000a120		\n"
	"mov sp,r0			\n"
	"pop {r4-r11}			\n"
	"pop {r0-r3}			\n"
	"pop {r12}			\n"
	"pop {lr}			\n"
	"pop {pc}			\n"
	);
}


void* tick_and_switch(void* cur_stack)
{
    void* temp ;
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
"    push {lr}			\n"
"    bl.w tick_and_switch 	\n"
"    pop {lr}			\n"
"				\n"
"    cmp r0,#0   		\n"
"    beq end			\n"
"				\n"
"    push {r4-r11}   		\n"
"    mov sp,r0       		\n"
"    pop  {r4-r11}   		\n"
"				\n"
"end:             		\n"
	);
}


