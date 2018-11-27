#include "atiny.h"
#include "atiny_log.h"


void atiny_sock_init(atiny_if_t *interface)
{
    (void)interface;
    ATINY_LOG(LOG_DEBUG, " using select()");
}

void atiny_sock_uninit(atiny_if_t *interface)
{

}

void atiny_sock_connect(atiny_connection_t *nc)
{
    int rc = 0;
    nc->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sock fd:%d\n", nc->sock_fd);
    if (nc->sock_fd == -1) {
      ATINY_LOG(LOG_ERR, "socket error");
      return;
    }

    rc = connect(nc->sock_fd, (struct sockaddr *)&nc->address, sizeof(nc->address));
    printf("connect rc:%d\n", rc);
    if(rc < 0)
      ATINY_LOG(LOG_ERR, "sock %d rc %d ",  nc->sock_fd, rc);
}

void atiny_sock_discon(atiny_connection_t *nc)
{


}


time_t atiny_sock_poll(atiny_if_t *interface, int timeout_ms)
{
    fd_set rfds, wfds, efds;
    atiny_manager_t *m = interface->mgr;
    struct timeval tv;
    int max_fd = -1;
    int rc = 0;

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    FD_SET(m->nc->sock_fd, &rfds);
    FD_SET(m->nc->sock_fd, &wfds);
    FD_SET(m->nc->sock_fd, &efds);

    max_fd = m->nc->sock_fd;

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    rc = select(max_fd + 1, &rfds, &wfds, &efds, &tv);

    if(rc == -1)
        printf("select() error\n");
    else if(rc)
    {
        printf("Data is available now.\n");
        if(FD_ISSET(m->nc->sock_fd, &rfds)) printf("r\n");
        if(FD_ISSET(m->nc->sock_fd, &wfds)) printf("w\n");
        if(FD_ISSET(m->nc->sock_fd, &efds)) printf("e\n");
        /* FD_ISSET(0, &rfds) will be true. */
    }



}

int atiny_sock_send(atiny_connection_t *nc, void *buf, size_t len)
{

}

int atiny_sock_recv(atiny_connection_t *nc, void *buf, size_t len)
{

}

atiny_if_funcs_t linux_sock = 
{
    atiny_sock_init,
    atiny_sock_uninit,
    atiny_sock_connect,
    atiny_sock_discon,
    atiny_sock_poll,
    atiny_sock_send,
    atiny_sock_recv,
};
