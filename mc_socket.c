void
setreuseaddr(mc_sock_fd fd) {
    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
}

int
mc_socket() {
    int retsock = socket(DEFAULT_NET, DEFAULT_DATA_GRAM, 0);
    if (retsock < 0)
    {
        return -1;
    }
    return retsock;
}

int
mc_bind(mc_sock_fd listenfd) {
    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(DEFAULT_PORT);
    return bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
}

int
mc_listen(mc_sock_fd listenfd) {
    return listen(listenfd, DEFAULT_BACKLOG);
}

void
handler_accept(int fd, short revent, void *args) {
    struct sockaddr_in in_addr;
    size_t in_len;
    int s;
    int done = 0;
    struct _connection *lc = (struct _connection *)args;

    in_len = sizeof(in_addr);
    mc_setnonblocking(fd);
    while (!done)
    {
        s = accept(fd, (struct sockaddr *)&in_addr, &in_len);
        if (s == -1)
        {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                break;
            else
            {
                perror("accept");
                break;
            }
        }

        if (s == 0)
        {
            fprintf(stderr, "Aceept a connection on %d \n", fd);
        }
        done = 1;
    }

    mc_setnonblocking(s);
    lc->fd = s;
    mc_event_set(&(lc->read), MC_EV_READ, lc->fd, handler_read, lc);
    mc_event_set(&(lc->write), MC_EV_WRITE, lc->fd, handler_write, lc);
    mc_event_post(&(lc->write), lc->base);
}

void
handler_read(int fd, short revent, void *args)
{
    mc_setnonblocking(fd);
    struct _connection *lc;
    lc = (struct _connection *)args;
    write(fd, lc->buf, 1024);
    mc_event_set(&(lc->read), MC_EV_READ, lc->fd, handler_read, lc);
}

void
cab(int fd, short revent, void *args) {
    mc_set_nonblocking(fd);
    char buf[1024] = "xx00xx00xx00xx00\n";
    write(fd, buf, 1024);
}
