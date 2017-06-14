#include "rtos.h"
#include "glib.h"
#include "systick.h"
#include "leds.h"
#include "timer.h"
#include "mem.h"

extern struct task_list *run;
struct os_timer_list *head_timer;


void os_delay_init(void)
{
    head_timer = malloc(sizeof(struct os_timer_list));
    head_timer->next = NULL;
}

void os_delay_ms(int time)
{
    struct os_timer_list *timer_st;
    if ((run->task->status) && (0x0001)) {
        return ;
    }
    timer_st = malloc(sizeof(struct os_timer_list));
    if(timer_st == NULL)
        return ;
    portDISABLE_INTERRUPTS();
    timer_st->next = head_timer->next;
    head_timer->next = timer_st;
    timer_st->timer_p.task_str = run;
    portENABLE_INTERRUPTS();

    timer_st->timer_p.count = time;
    if(timer_st->timer_p.count != 0) {
        timer_st->timer_p.task_str->task->status |= 0x0001;
    }
}

void os_delay_clear(void)
{
    struct os_timer_list *free_p;
    struct os_timer_list *temp;
    struct os_timer_list *timer_temp = head_timer;
    while(timer_temp->next) {
        temp = timer_temp;
        timer_temp = timer_temp->next;
        timer_temp->timer_p.count--;
        if (timer_temp->timer_p.count <= 0) {
            timer_temp->timer_p.task_str->task->status &= 0xfffe;

        portDISABLE_INTERRUPTS();
            temp->next = timer_temp->next;
            free_p = timer_temp;
            timer_temp = temp->next;
        portENABLE_INTERRUPTS();

            free(free_p);
        }
    }
}



