#include "mc_event.h"
#include "mc_socket.h"

int
main() {
    mc_event_t mev;
    mc_event_base_t *base = mc_base_new();
    struct _connection lc;
    lc.base = base;

    int sockfd = mc_socket();
    mc_bind(sockfd);
    mc_listen(sockfd);
    
    mc_event_set(&(lc.read), MC_EV_READ, sockfd, handler_accept, &lc);
    mc_event_post(&(lc.read), base);
    mc_dispatch(base);

    return 0;
}
