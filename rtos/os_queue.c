/*
 * message
 *
 */

#include "rtos.h"

int send_msg_queues(int recv_id, void *msg)
{
    int count;
    struct task_p *recv_task;
    struct msg_queues_list *queues_list;

    recv_task = find_task_by_id(recv_id);
    count = recv_task->msg_list.msg_count;
    queues_list = recv_task->msg_list.head;
    
    while (count--) {
        queues_list = queues_list->next;
    }
    
    queues_list->next = malloc(sizeof(struct msg_queues_list));
    queues_list->next->next = NULL;
    queues_list->next->msg.recv_id = recv_id;
    queues_list->next->msg.data = msg;
    queues_list = queues_list->next;

    recv_task->status |= 0x0002;
    return 0;
}

