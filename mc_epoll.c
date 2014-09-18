void*
mc_epoll_init(mc_event_base_t *meb) {
    if (meb->magic != MC_BASE_MAGIC)
    {
        fprintf(stderr, "In function mc_epoll_init %d, %s", __LINE__, __FILE__);
        return NULL;
    }

    meb->epoll_fd = epoll_create(MC_EVENT_MAX);
    
    return meb;
}

int
mc_epoll_add(void* arg, mc_event_t *ev) {
    if (ev->base->magic != MC_BASE_MAGIC)
    {
        fprintf(stderr, "In function mc_epoll_add %d, %s", __LINE__, __FILE__);
        return -1;
    }

    mc_event_base_t *base = ev->base;
    int epoll_fd = base->epoll_fd;
    int err;

    struct epoll_event epoll_ev;
    epoll_ev.data.ptr = ev;
    epoll_ev.events = EPOLLIN | EPOLLET;

    if (!(ev->ev_flags & MC_EV_ADDED))
    {
        err = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev->ev_fd, &epoll_ev);
        if (err != 0)
        {
            perror("epoll_ctl");
            fprintf(stderr, "In function mc_epoll_add the epoll_ctl error in file: %s, line:%d\n", __FILE__, __LINE__);
            return -1;
        }
        ev->ev_flags |= MC_EV_ADDED;
    }
    return 0;
}

int
mc_epoll_del(void* arg, mc_event_t *ev) {
    if (ev->base->magic != MC_BASE_MAGIC)
    {
        fprintf(stderr, "In function mc_epoll_add %d, %s", __LINE__, __FILE__);
        return -1;
    }

    mc_event_base_t *base = ev->base;
    int epoll_fd = base->epoll_fd;
    int err;
    
    if (!(ev->ev_flags & MC_EV_INITD))
    {
        return -1;
    }
    err = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev->ev_fd, NULL);
    if (err != 0)
    {
        fprintf(stderr, "In function mc_epoll_del the epoll_ctl error in file:%s, line:%d\n", __FILE__, __LINE__);
        return -1;
    }
    ev->ev_flags = 0x0000;
    return 0;
}

int
mc_epoll_mod(void* arg, mc_event_t *ev) {
    if (ev->base->magic != MC_BASE_MAGIC)
    {
        fprintf(stderr, "In function mc_epoll_add %d, %s", __LINE__, __FILE__);
        return -1;
    }

    mc_event_base_t *base = ev->base;
    int epoll_fd = base->epoll_fd;
    int err;
    unsigned int mode;

    if (!(ev->ev_flags & MC_EV_INITD))
    {
        return -1;
    }

    struct epoll_event epoll_ev;
    epoll_ev.data.ptr = ev;
    if (arg == NULL)
    {
        return 0;
    }

    mode = *(unsigned int *)arg;
    epoll_ev.events = mode;
    err = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ev->ev_fd, &epoll_ev);
    if (err != 0)
    {
        fprintf(stderr, "In function mc_epoll_del the epoll_ctl error in file: %s, line: %d", __FILE__, __LINE__);
        return -1;
    }
    return 0;
}

int
mc_epoll_loop(void* args, mc_event_base_t *base, struct timeval ev_time) {
    if (base == NULL)
    {
        fprintf(stderr, "base == NULL in mc_epoll_loop in file:%s, line:%d, __FILE__, __LINE__");
        return -1;
    }

    if (base->magic != MC_BASE_MAGIC)
    {
        fprintf(stderr, "In function mc_epoll_mod in file:%s, line:%d", __FILE__, __LINE__);
        return -1;
    }
    
    struct epoll_event *nevents = (struct epoll_event *) args;
    struct epoll_event epoll_ev;

    nfds = epoll_wait(base->epoll_fd, nevents, MC_EVENT_MAX, 1);
    if (nfds <= -1)
    {
        fprintf(stderr, "epoll wait function in mc_epoll_loop in file: %s, line: %d\n", __FILE__, __LINE__);
        return nfds;
    }

    return nfds;
}
