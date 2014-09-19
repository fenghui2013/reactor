#include "mc_event.h"

mc_event_base_t*
mc_base_new(void) {
    mc_event_base_t *base = (mc_event_base_t *)malloc(sizeof(mc_event_base_t));
    if (base == NULL)
    {
        fprintf(stderr, "Init the base module in mc_base_new error in file: %s, line: %d", __FILE__, __LINE__);
        return NULL;
    }

    /* init the base lists*/
    base->added_list = NULL;
    base->active_list = NULL;
    base->magic = MC_BASE_MAGIC;
    base->event_num = 0;
    base->event_active_num = 0;
    base->ev_base_stop = MC_BASE_STOP;
    
    gettimeofday(&base->event_time, NULL);
    mc_event_init(base);

    return base;
}

int
mc_event_set(mc_event_t *ev, short revent, int fd, mc_ev_callback callback, void *args) {
    if (ev == NULL)
    {
        fprintf(stderr, "mc_event_set error, ev == NULL or other segment error in file: %s, line: %d", __FILE__, __LINE__);
        return -1;
    }
#if (HAVE_EPOLL)
    unsigned int epoll_flag;
#endif
    int err;
    memset(ev, 0, sizeof(mc_event_t));
    ev->next = NULL;
    ev->prev = NULL;
    ev->callback = callback;
    ev->ev_fd = fd;
    ev->revent = revent;
    ev->args = args;

    /* this job post to mc_event_post
    if (revent & MC_EV_LISTEN)
    {
        err = mc_event_add(NULL, ev);
        if (err != 0)
            fprintf(stderr, "mc_event_add in mc_event_set \n");
    }
    */
#if (HAVE_EPOLL)
    if (revent & MC_EV_READ)
    {
        epoll_flag = EPOLLIN | EPOLLET;
        err = mc_event_mod((void *)&epoll_flag, ev);
        if (err != 0)
            fprintf(stderr, "mc_event_mod (MC_EVENT_READ) in mc_event_set in file: %s, line: %d\n", __FILE__, __LINE__);
    }
    if (revent & MC_EV_WRITE)
    {
        epoll_flag = EPOLLOUT | EPOLLET;
        err = mc_event_mod((void *)&epoll_flag, ev);
        if (err != 0)
            fprintf(stderr, "mc_event_mod (MC_EVENT_WRITE) in mc_event_set in file: %s, line: %d\n", __FILE__, __LINE__);
    }

    ev->ev_flags |= MC_EV_INITD;
#endif

    return 0;
}

int
mc_event_post(mc_event_t *ev, mc_event_base_t *base)
{
    if (ev == NULL || base == NULL)
    {
        fprintf(stderr, "In function mc_event_post, the_args error, please check your argument in file: %s, lien:%d\n", __FILE__, __LINE__);
        return -1;
    }

    if (base->magic != MC_BASE_MAGIC)
    {
        fprintf(stderr, "The mc_event_base_t * points base non inited in mc_event_post in file: %s, lien:%d\n", __FILE__, __LINE__);
        return -1;
    }

    int err;
    ev->base = base;
    add_event_to_queue(ev, (mc_event_t **)&(base->added_list));
    base->event_num++;

    err = mc_event_add(NULL, ev);
    if (err == -1)
    {
        fprintf(stderr, "In function mc_event_add error in file: %s, lien:%d\n", __FILE__, __LINE__);
        return -1;
    }
}

int
mc_dispatch(mc_event_base_t *base) {
    if (base == NULL)
    {
        fprintf(stderr, "base == NULL in function mc_dispatch in file: %s, lien:%d\n", __FILE__, __LINE__);
        return -1;
    }

    if (base->magic != MC_BASE_MAGIC)
    {
        fprintf(stderr, "In function mc_dispatch noinitlized in file: %s, lien:%d\n", __FILE__, __LINE__);
        return -1;
    }

    struct epoll_event *nevents = (struct epoll_event *)malloc(sizeof(struct epoll_event));

    int done = 0;
    int nevent;
    int i;
    mc_event_t *levent;
    mc_event_t *retevent;

    while (!done)
    {
        nevent = mc_event_loop(nevents, base, base->event_time);
        if (nevent == -1)
        {
            fprintf(stderr, "No event check, return in file: %s line:%d\n", __FILE__, __LINE__);
            goto err1;
        }

        for (i = 0; i < nevent; i++)
        {
            if ((nevents[i].events & EPOLLERR) || (nevents[i].events & EPOLLHUP))
            {
                levent = nevents[i].data.ptr;
                if (!(levent->ev_flags & MC_EV_INITD))
                    continue;
                if ((levent->ev_flags & MC_EV_ACTIVE) || (levent->ev_flags & MC_EV_ADDED))
                    del_event_from_queue(levent);
            }
            
            if (nevents[i].events & EPOLLIN)
            {
                levent = nevents[i].data.ptr;
                levent->revent = MC_EV_READ;
                add_event_to_queue(levent, (mc_event_t **)&(base->active_list));
                levent->ev_flags |= MC_EV_ACTIVE;
                base->event_active_num++;
            }
            else if (nevents[i].events & EPOLLOUT)
            {
                levent = nevents[i].data.ptr;
                levent->revent = MC_EV_WRITE;
                add_event_to_queue(levent, (mc_event_t **)&(base->active_list));
                levent->ev_flags |= MC_EV_ACTIVE;
                base->event_active_num++;
            }
            else
            {
                fprintf(stderr, "Unkonw err in file: %s, line: %d\n", __FILE__, __LINE__);
                goto err1;
            }
        }

        retevent = (mc_event_t *)(base->active_list);
        for (i = 0; i < nevnet; i++)
        {
            fprintf(stderr, "%d event(s)\n", nevent);
            if (retevent == NULL)
                break;
            retevent = get_event_and_del((mc_event_t *)(base->active_list));
            /*if we want to reuse this event we should set event again*/
            retevent->ev_flags = retevent->ev_flags & (~MC_EV_ACTIVE);
            base->event_active_num--;
            if (retevent == NULL)
                fprintf(stderr, "event is NULL file: %s, line: %d\n", __FILE__, __LINE__);

            retevent->callback(retevent->ev_fd, retevent->revent, retevent->args);
        }
    }
    return 0;
err1:
    return -1;
}
