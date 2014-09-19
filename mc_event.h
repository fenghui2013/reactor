#ifndef _MC_EVENT_H_
#define _MC_EVENT_H_

#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define HAVE_EPOLL    1

/* the magic value in mc_event_base_t*/
#define MC_BASE_STOP    0x0000
#define MC_BASE_ACTIVE  0x0001
#define MC_BASE_MAGIC   0x1989
#define MC_EVENT_MAX    10240

typedef struct mc_event_base_s
{
    void                *added_list;
    void                *active_list;
    unsigned int        event_num;
    unsigned int        event_active_num;
#if (HAVE_EPOLL)
    int                 epoll_fd;                // for epoll only
#endif
    int                 ev_base_stop;
    int                 magic;
    struct timeval      event_time;
} mc_event_base_t;

/* the revent value in mc_event_s*/
#define MC_EV_READ    0x0001
#define MC_EV_WRITE   0x0002
#define MC_EV_SIGNAL  0x0004
#define MC_EV_TIMEOUT 0x0008
#define MC_EV_LISTEN  0x0010

/* the ev_flags value in mc_event_s */
#define MC_EV_INITD   0x0001
#define MC_EV_ADDED   0x0002
#define MC_EV_ACTIVE  0x0004
#define MC_EV_DELED   0x0008

typedef void (*mc_ev_callback)(int, short, void *);

//event
typedef struct mc_event_s
{
    struct mc_event_s   *next;
    struct mc_event_s   *prev;

    mc_event_base_t     *base;

    mc_ev_callback      callback;                  // callback of this event
    int                 ev_fd;                     // file description of event
    short               revent;                    // event type
    void                *args;

    unsigned int        min_heap_index;
    struct              timeval ev_timeval;        // event timeout time

    int                 ev_flags;                  // event flags
} mc_event_t;

typedef struct mc_event_ops
{
    void* (*init)(mc_event_base_t *);
    int   (*add)(void *, mc_event_t *);
    int   (*del)(void *, mc_event_t *);
    int   (*mod)(void *, mc_event_t *);
    int   (*dispatch)(void *, mc_event_base_t *, struct timeval);
} mc_event_opt;

extern mc_event_opt mc_event_op_val;

#define mc_event_init  mc_event_op_val.init
#define mc_event_add   mc_event_op_val.add
#define mc_event_del   mc_event_op_val.del
#define mc_event_mod   mc_event_op_val.mod
#define mc_event_loop  mc_event_op_val.dispatch

mc_event_base_t* mc_base_new(void);
int mc_event_set(mc_event_t *ev, short revent, int fd, mc_ev_callback callback, void *args);
int mc_event_post(mc_event *ev, mc_event_base_t *base);
int mc_dispatch(mc_event_base_t *base);

void add_event_to_queue(mc_event_t *ev, void*);
mc_event_t* get_event_and_del(mc_event_t * ev);

#endif
