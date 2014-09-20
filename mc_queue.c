#include "mc_queue.h"

mc_queue_t *
mc_queue_middle(mc_queue_t *queue) {
    mc_queue_t *middle, *next;

    middle = mc_queue_head(queue);
    if (middle == mc_queue_last(queue)) return middle;

    next = mc_queue_head(queue);
    for (;;)
    {
        middle = mc_queue_next(middle);
        next = mc_queue_next(next);
        if (next == mc_queue_last(queue)) return middle;
        next = mc_queue_next(next);
        if (next == mc_queue_last(queue)) return middle;
    }
}

void
mc_queue_sort(mc_queue_t *queue,
        int (*cmp)(const mc_queue_t *, const mc_queue_t *)) {
    mc_queue_t *q, *prev, *next;
    q = mc_queue_head(queue);

    if (q == mc_queue_last(queue))
        return;

    for (q = mc_queue_next(q); q != mc_queue_sentinel(queue); q = next)
    {
        prev = mc_queue_prev(q);
        next = mc_queue_next(q);

        mc_queue_remove(q);
        do
        {
            if (cmp(prev, q) <= 0)
                break;
            prev = mc_queue_prev(prev);
        } while(prev != mc_queue_sentinel(queue));

        mc_queue_insert_after(prev, q);
    }
}
