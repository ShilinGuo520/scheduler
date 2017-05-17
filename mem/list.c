#include "mem.h"

struct list *malloc_list(int size)
{
    struct list *p = NULL;
    struct list *head;
    head = malloc(sizeof(struct list));
    p = head;

    while (size--) {
        p->next = malloc(sizeof(struct list));
        p = p->next;
    };
    return head;
}

int free_list(struct list *head)
{
    int num = 0;
    struct list *p;
    while (head->next) {
        p = head;
        head = head->next;
        free(p);
        num++;
    }
    return num;
}

