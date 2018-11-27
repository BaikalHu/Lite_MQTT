#include <pthread.h>
#include <unistd.h>
#include "atiny_log.h"
#include "atiny.h"

static atiny_device_info_t default_dev_info;
extern atiny_if_funcs_t linux_sock;


int main()
{
    atiny_manager_t mgr;
    atiny_connect_param_t param;
    param.proto_type = SOCK_STREAM;
    param.server_ip = "127.0.0.1";
    param.server_port = 1883;

    default_dev_info.ifuncs = &linux_sock;
    
    printf("1\n");
    atiny_init(&mgr, &default_dev_info);
    printf("2\n");

    atiny_connect_with_param(&mgr, NULL, param);
    for(;;)
    {
        atiny_poll(&mgr, 1000);
    }
}


