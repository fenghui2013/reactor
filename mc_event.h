#ifndef _MC_EVENT_H_
#define _MC_EVENT_H_

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

//event
typedef struct mc_event_s
{
    struct mc_event_s   *next;
    struct mc_event_s   *prev;
    unsigned int        min_heap_index;
    int                 ev_fd;                     // file description of event
    short               revent;                    // event type
    struct              timeval ev_timeval;        // event timeout time
    mc_ev_callback      callback;                  // callback of this event
    void                *args;
    int                 ev_flags;                  // event flags
    mc_event_base_t     *base;
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

#endif
