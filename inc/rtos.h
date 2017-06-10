#include "types.h"

#define PRIORITY_MAX    10
#define RT              0

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



struct msg_queues {
    int recv_id;
    int send_id;
    void *data;
};

struct msg_queues_list {
    struct msg_queues msg;
    struct msg_queues_list *next;
};

struct task_msg {
    int msg_count;
    struct msg_queues_list *head;
};

struct task_p {
    long id;
    long priority;
    struct task_msg msg_list;
    unsigned long status;
    unsigned char *stack;
    struct task_init_stack_frame *base;
    void *basep;
};

struct task_list {
    struct task_p *task;
    struct task_list *next;
};

struct os_timer {                                                                                                                                      
        long count;
        struct task_list *task_str;
};

struct os_timer_list {
        struct os_timer timer_p;
        struct os_timer_list *next;
};


extern void rtos_start(void);
extern void task_init(void);
extern int creat_task(void (*func), int stack_size, int priority);

extern void os_delay_init(void);
extern void os_delay_clear(void);
extern void os_delay_ms(int time);

extern struct task_list *find_task_by_id(int task_id);
extern struct task_list *get_run_task(void);

extern int send_msg_queues(int recv_id, void *msg);
extern void *recv_msg_queues(void);

/*
 * Set basepri to portMAX_SYSCALL_INTERRUPT_PRIORITY without effecting other
 * registers.  r0 is clobbered.
 */ 
#define portSET_INTERRUPT_MASK()						\
	__asm volatile								\
	(									\
		"cpsid	i					\n"	\
	)
	
/*
 * Set basepri back to 0 without effective other registers.
 * r0 is clobbered.
 */
#define portCLEAR_INTERRUPT_MASK()						\
	__asm volatile								\
	(									\
		"cpsie	i					\n"	\
	)

#define portDISABLE_INTERRUPTS()	portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()		portCLEAR_INTERRUPT_MASK()


