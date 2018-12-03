#include <string.h>

#include "atiny.h"




atiny_time_t atiny_gettime_ms(void)
{

    struct timeval tv;
    gettimeofday(&tv,NULL);

    return tv.tv_sec*1000 + tv.tv_usec/1000;
}

void *atiny_malloc(size_t size)
{
    return malloc(size);
}

void atiny_buf_init(atiny_buf_t *abuf, size_t size)
{
    abuf->size = size;
    abuf->len = 0;
	abuf->data = atiny_malloc(size);
}



int atiny_parse_address(struct sockaddr_in *addr, char *server_ip, unsigned int server_port)
{
    int rc = -1;
    struct addrinfo *result = NULL;
    struct addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};

    if ((rc = getaddrinfo(server_ip, NULL, &hints, &result)) == 0)
    {
        struct addrinfo *res = result;

        while (res)
        {
            if (res->ai_family == AF_INET)
            {
                result = res;
                break;
            }
            res = res->ai_next;
        }

        if (result->ai_family == AF_INET)
        {
            addr->sin_port = htons((unsigned short)server_port);
            addr->sin_family = AF_INET;
            addr->sin_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr;
        }
        else
            rc = -1;

        freeaddrinfo(result);
    }

    return rc;
}


int atiny_init(atiny_manager_t *m, atiny_device_info_t *param)
{
    m->interface = (atiny_if_t *)atiny_malloc(sizeof(atiny_if_t));
    m->interface->mgr = m;
    m->interface->ifuncs = param->ifuncs;
    m->interface->ifuncs->init(m->interface);
}



atiny_connection_t* atiny_connect(atiny_manager_t *m, atiny_event_handler cb)
{
  atiny_connect_param_t params;
  memset(&params, 0, sizeof(params));
  return atiny_connect_with_param(m, cb, params);
}

atiny_connection_t* atiny_connect_with_param(atiny_manager_t *m, atiny_event_handler cb, atiny_connect_param_t param)
{
    atiny_connection_t *nc = NULL;
    if((nc = (atiny_connection_t *)atiny_malloc(sizeof(atiny_connection_t))) != NULL)
    {
        nc->mgr = m;
        m->nc = nc;
        nc->flags |= (param.proto_type == SOCK_STREAM) ? 0 : ATINY_FG_UDP;
        nc->flags |= ATINY_FG_CONNECTING;
        nc->sock_fd = -1;
        nc->user_handler = cb;
        nc->proto_handler = NULL;
        nc->last_time = atiny_gettime_ms();
		atiny_buf_init(&(nc->send_buf), 1024);
		atiny_buf_init(&(nc->recv_buf), 1024);
        atiny_parse_address(&(nc->address), param.server_ip, param.server_port);
    }
    m->interface->ifuncs->connect(nc);
    return nc;
}

void atiny_poll(atiny_manager_t *m, int timeout_ms)
{
    m->interface->ifuncs->poll(m->interface, timeout_ms);
}
