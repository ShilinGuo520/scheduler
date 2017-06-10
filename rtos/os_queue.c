/*
 * message
 *
 */

#include "rtos.h"
#include "mem.h"

int send_msg_queues(int recv_id, void *msg)
{
    int count;
    struct task_list *recv_task;
    struct msg_queues_list *queues_list;

    recv_task = find_task_by_id(recv_id);
    count = recv_task->task->msg_list.msg_count;
    queues_list = recv_task->task->msg_list.head;
    
    while (count--) {
        queues_list = queues_list->next;
    }
    
    queues_list->next = malloc(sizeof(struct msg_queues_list));
    queues_list->next->next = NULL;
    queues_list->next->msg.recv_id = recv_id;
    queues_list->next->msg.data = msg;
    queues_list = queues_list->next;


    recv_task->task->status &= ~(0x0002);
    recv_task->task->msg_list.msg_count++;

    return 0;
}

void *recv_msg_queues(void)
{
    void *msg;
    struct msg_queues_list *recv_queues;
    struct task_list *recv_task = get_run_task();
    if (recv_task->task->status && (0x0002)) {
        return NULL;
    } else {
        if (recv_task->task->msg_list.msg_count) {
            msg = recv_task->task->msg_list.head->msg.data;
            recv_queues = recv_task->task->msg_list.head;
            recv_task->task->msg_list.head = recv_task->task->msg_list.head->next;
            recv_task->task->msg_list.msg_count--;
            return msg;
        } else {
            recv_task->task->status |= 0x0002;
            return NULL;
        }
    }
}


