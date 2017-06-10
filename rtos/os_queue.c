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

    recv_task->task->status |= 0x0002;
    return 0;
}

