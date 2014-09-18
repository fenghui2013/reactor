#define HAVE_EPOLL    1

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

mc_event_base_t* mc_base_new(void);
int mc_event_set(mc_event_t *ev, short revent, int fd, mc_ev_callback callback, void *args);
int mc_event_post(mc_event *ev, mc_event_base_t *base);
int mc_dispatch(mc_event_base_t *base);
