#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head)
{
    /* your code here */
    if (!head || !head->next) //head is null or head has no next
    {
        return 0;
    }
    node *tortoise = head;
    node *hare = head;
    while (hare->next && hare->next->next)
    {
        hare = hare->next->next;
        tortoise = tortoise->next;
        if (tortoise == hare)
        {
            return 1;
        }
    }
    return 0;
}