#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>



#define ATINY_FG_UDP (1 << 0)
#define ATINY_FG_CONNECTING (1 << 1)

#define ATINY_FG_CAN_RD (1 << 2)
#define ATINY_FG_CAN_WR (1 << 3)
#define ATINY_FG_ERR    (1 << 4)

#define ATINY_EV_POLL       1
#define ATINY_EV_CONNECTED  2
#define ATINY_EV_SEND       3
#define ATINY_EV_RECV       4
#define ATINY_EV_CLOSE      5


typedef struct atiny_connection atiny_connection_t;
typedef struct atiny_if atiny_if_t;
typedef struct atiny_buf atiny_buf_t;
typedef struct atiny_if_funcs atiny_if_funcs_t;
typedef struct atiny_manager atiny_manager_t;

typedef void (*atiny_event_handler)(atiny_connection_t *nc, int event, void *event_data);

typedef struct atiny_buf
{
    char *data;
    size_t len;    //data len
    size_t size;   //total size
} atiny_buf_t;

typedef struct atiny_manager
{
    atiny_connection_t *nc;
    atiny_if_t *interface;
} atiny_manager_t;

typedef struct atiny_connection
{
    atiny_manager_t *mgr;
    int flags;
    int sock_fd;
    struct sockaddr_in address;
    atiny_buf_t recv_buf;
    atiny_buf_t send_buf;
    time_t last_time;
    atiny_event_handler user_handler;
    atiny_event_handler proto_handler;
#ifdef WITH_DTLS
    void *ssl_handler;
#endif
} atiny_connection_t;

typedef struct atiny_if
{
    atiny_manager_t *mgr;
    atiny_if_funcs_t *ifuncs;
} atiny_if_t;

typedef struct atiny_if_funcs
{
    void (*init)(atiny_if_t *interface);
    void (*uninit)(atiny_if_t *interface);
    void (*connect)(atiny_connection_t *nc);
    void (*discon)(atiny_connection_t *nc);
    time_t (*poll)(atiny_if_t *interface, int timeout_ms);
    int (*send)(atiny_connection_t *nc, void *buf, size_t len);
    int (*recv)(atiny_connection_t *nc, void *buf, size_t len);
} atiny_if_funcs_t;

typedef struct atiny_device_info
{
//    char *server_ip;
//    unsigned int server_port;
    atiny_if_funcs_t *ifuncs;
} atiny_device_info_t;

typedef struct atiny_connect_param
{
    int proto_type;
    char *server_ip;
    unsigned int server_port;
//    atiny_if_funcs_t *ifuncs;
#ifdef WITH_DTLS
    // add some param used in dtls, like ca, key etc.
#endif
} atiny_connect_param_t;

int atiny_init(atiny_manager_t *m,  atiny_device_info_t *param);
atiny_connection_t* atiny_connect(atiny_manager_t *m, atiny_event_handler cb);
atiny_connection_t* atiny_connect_with_param(atiny_manager_t *m, atiny_event_handler cb, atiny_connect_param_t param);
void atiny_poll(atiny_manager_t *m, int timeout_ms);


