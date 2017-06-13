/*
 * message
 *
 */

#include "rtos.h"
#include "mem.h"
#include "glib.h"

int send_msg_queues(int recv_id, void *msg)
{
    int count;
    struct task_list *recv_task;
    struct msg_queues_list *queues_list;
    struct msg_queues_list *queues_list_head;
    struct msg_queues_list *queues_list_last;
    recv_task = find_task_by_id(recv_id);
    queues_list_head = recv_task->task->msg_list.head;
    queues_list_last = recv_task->task->msg_list.last;

    queues_list = malloc(sizeof(struct msg_queues_list));
    queues_list->next = NULL;
    queues_list->msg.recv_id = recv_id;
    queues_list->msg.data = msg;
portDISABLE_INTERRUPTS();
    if (recv_task->task->msg_list.head == NULL) {
	recv_task->task->msg_list.head = queues_list;
        recv_task->task->msg_list.last = queues_list;
    } else {
	recv_task->task->msg_list.last->next = queues_list;
	recv_task->task->msg_list.last = recv_task->task->msg_list.last->next;
    }
portENABLE_INTERRUPTS();
    recv_task->task->status &= ~(0x0002);

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
        if (recv_task->task->msg_list.head != NULL) {
            msg = recv_task->task->msg_list.head->msg.data;
            recv_queues = recv_task->task->msg_list.head;
	    portDISABLE_INTERRUPTS();
            recv_task->task->msg_list.head = recv_task->task->msg_list.head->next;
	    portENABLE_INTERRUPTS();
	    free(recv_queues); 
            return msg;
        } else {
            recv_task->task->status |= 0x0002;
            return NULL;
        }
    }
}


