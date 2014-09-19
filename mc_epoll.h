#ifndef _MC_EPOLL_H_
#define _MC_EPOLL_H_

#include <stdio.h>
#include <sys/epoll.h>
#include "mc_event.h"

void* mc_epoll_init(mc_event_base_t *meb);
int mc_epoll_add(void* arg, mc_event_t *ev);
int mc_epoll_del(void* arg, mc_event_t *ev);
int mc_epoll_mod(void* arg, mc_event_t *ev);
int mc_epoll_loop(void* args, mc_event_base_t *base, struct timeval ev_time);

#endif
