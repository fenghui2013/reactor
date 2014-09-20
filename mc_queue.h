#ifndef _MC_QUEUE_H_
#define _MC_QUEUE_H_

#include <stddef.h>
#include <sys/types.h>

typedef struct mc_queue_s mc_queue_t;

struct mc_queue_s
{
    mc_queue_t *prev;
    mc_queue_t *next;
};

#define mc_queue_init(q)                      \
    (q)->prev = q;                            \
    (q)->next = q

#define mc_queue_empty(h)                     \
    (h == (h)->prev)

#define mc_queue_insert_head(h, x)            \
    (x)->next = (h)->next;                    \
    (x)->next->prev = (x);                    \
    (x)->prev = h;                            \
    (h)->next = x

#define mc_queue_insert_after  mc_queue_insert_head

#define mc_queue_insert_tail(h, x)            \
    (x)->prev = (h)->prev;                    \
    (x)->prev->next = x;                      \
    (x)->next = h;                            \
    (h)->prev = x

#define mc_queue_head(h)                      \
    (h)->next

#define mc_queue_last(h)                      \
    (h)->prev

#define mc_queue_sentinel(h)                  \
    (h)

#define mc_queue_next(h)                      \
    (h)->next

#define mc_queue_prev(h)                      \
    (h)->prev

#define mc_queue_remove(x)                    \
    (x)->prev->next = (x)->next;              \
    (x)->next->prev = (x)->prev

#define mc_queue_split(h, q, n)               \
    (n)->prev = (h)->prev;                    \
    (n)->prev->next = n;                      \
    (n)->next = q;                            \
    (h)->prev = (n)->prev;                    \
    (h)->prev->next =  h;                     \
    (q)->prev = n

#define mc_queue_add(h, n)                    \
    (h)->prev->next = (n)->next;              \
    (n)->next->prev = (h)->prev;              \
    (h)->prev = (n)->prev;                    \
    (n)->prev->next = h

#define mc_queue_data(q, type, link)          \
    (type *) ((u_char *)q - offsetof(type, link))

mc_queue_t * mc_queue_middle(mc_queue_t *queue);
void mc_queue_sort(mc_queue_t *queue,
        int (*cmp)(const mc_queue_t *, const mc_queue_t *));

#endif
