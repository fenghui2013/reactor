#ifndef _MC_SOCKET_H_
#define _MC_SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include "mc_event.h"

#define mc_sock_fd int

#define DEFAULT_NET         AF_INET
#define DEFAULT_DATA_GRAM   SOCK_STREAM
#define DEFAULT_PORT        (1115)
#define DEFAULT_BACKLOG     (200)

struct _connection
{
    int              fd;
    mc_event_t       read;
    mc_event_t       write;
    char             buf[1024];
    mc_event_base_t  *base;
};

void setreuseaddr(mc_sock_fd fd);
int mc_socket();
int mc_bind(mc_sock_fd listenfd);
int mc_listen(mc_sock_fd listenfd);
void handler_accept(int fd, short revent, void *args);
void handler_read(int fd, short revent, void *args);
void handler_write(int fd, short revent, void *args);
void cab(int fd, short revent, void *args);

#endif
