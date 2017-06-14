#include "rtos.h"
#include "glib.h"
#include "systick.h"
#include "leds.h"
#include "timer.h"
#include "mem.h"


#define IDLE_TASK_STACK_SIZE        256


void idle_task(void)
{
    int i = 0;
    while(1) {
//      printf("idle task i=%d\n\r", i++);
    }
}


struct task_list head;
struct task_list *run;
int task_id_count;


struct task_list *get_run_task(void)
{
    return run;
}

struct task_list *find_task_by_id(int task_id)
{
    int id;
    struct task_list *ret = &head;
    id = task_id_count + 1 - task_id;
    while((id--) && (ret->next)) {
        ret = ret->next;
    }
    return ret;
}


#if RT
struct task_list *find_ready_task(struct task_list *task_run)
{
    struct task_list *ret_task = task_run->next;
    while(1) {
        if (ret_task->task->status) {
            ret_task = ret_task->next;
        } else {
            return ret_task;
        }
    }
}
#else
struct task_list *find_ready_task(struct task_list *task_run)
{
    int count;
    long cycle = 0;
    struct task_list *ret_task;

    while (cycle <= PRIORITY_MAX) {
        count = task_id_count;
        ret_task = task_run->next;
        while (count--) {
           if (ret_task->task->priority == cycle) {
                if(ret_task->task->status) {
                    ret_task = ret_task->next;
                } else {
                    return ret_task;
                }
           } else {
                ret_task = ret_task->next;
           }
        }
        cycle++;
    }
    return ret_task;
}
#endif

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

    printf("add:0x%x \n\r",task->basep);

    portDISABLE_INTERRUPTS();
    head.task = task;
        head.next = &head;
    run = &head;
    task_id_count = 0;
    task->id = task_id_count;
    portENABLE_INTERRUPTS();    
    
    task->status = 0x0000;
    task->priority = PRIORITY_MAX;
}

int creat_task(void (*func), int stack_size, int priority)
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

    task->msg_list.head = NULL;
    task->msg_list.last = NULL;

    p = malloc(sizeof(struct task_list));

    portDISABLE_INTERRUPTS();
    p->next = head.next;
    head.next = p;
    p->task = task;
    task_id_count++;
    task->id = task_id_count;
    portENABLE_INTERRUPTS();

    task->status = 0x0000;
    priority = MAX(0, priority);
    task->priority = MIN(PRIORITY_MAX, priority);
    return (task->id);
}

void rtos_start(void)
{
    os_delay_init();
    systick_init(71999);

__asm volatile (
    "ldr r0,=0x2000a131     \n"
    "mov sp,r0          \n"
    "pop {r4-r11}           \n"
    "pop {r0-r3}            \n"
    "pop {r12}          \n"
    "pop {lr}           \n"
    "pop {pc}           \n"
    );
}


void* tick_and_switch(void* cur_stack)
{
    void* temp ;
    os_delay_clear();       //clear task delay timer
    run->task->basep = cur_stack;
    run = find_ready_task(run);
    temp = run->task->basep;
    return temp;
}


void SysTick_Handler()
{
__asm volatile (
"    mov r0, sp             \n"
"    sub r0, #(8*4)         \n"
"    push {lr}          \n"
"    bl.w tick_and_switch   \n"
"    pop {lr}           \n"
"               \n"
"    cmp r0,#0          \n"
"    beq end            \n"
"               \n"
"    push {r4-r11}          \n"
"    mov sp,r0              \n"
"    pop  {r4-r11}          \n"
"               \n"
"end:                   \n"
    );
}


