#include "rtos.h"
#include "glib.h"
#include "systick.h"
#include "leds.h"
#include "timer.h"
#include "mem.h"


void task_init(void);

void task_print(void)
{
	int t = 0;
	char *p;
	char *p1;
	int i;
	while(1) {
		i = 0;
		printf("task_print %d \n\r",t++);
		p = malloc(100);
		p1 = p;
		printf("Debug# line: %d \n\r", __LINE__);
		printf("Debug# p=0x%x \n\r", p);
		while(i < 100) {
			printf("Debug# line: %d \n\r", __LINE__);
			*p = i++;
			p++;
		}
		while(i--) {
			printf("p[%d] = %d \n\r", i, *p);
			p--;
		}
		free(p1);
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

struct task_init_stack_frame dummya[10] = {0};
struct task_init_stack_frame dummy;
struct task_init_stack_frame base;
struct task_init_stack_frame basea[10] = {0};

void* next_stack;

void task_init(void)
{
	base.r0 = 0;
	base.pc = (ins_ptr)(&task_print);
	base.lr = 0;
	base.xpsr = 0x61000000;

	next_stack = &base;
}



void* tick_and_switch(void* cur_stack)
{
    void* temp ;

	temp = next_stack;
    next_stack = cur_stack;
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

