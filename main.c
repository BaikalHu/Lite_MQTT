#include <pthread.h>
#include <unistd.h>
#include "atiny_log.h"
#include "atiny.h"
#include "atiny_mqtt.h"

static atiny_device_info_t default_dev_info;
extern atiny_if_funcs_t linux_sock;

void ev_handler(atiny_connection_t *nc, int event, void *event_data)
{
    printf("ev_handler in main\n");
	char buf[36] = {0x10, 0x22, 0x00, 0x06,0x4d, 0x51, 0x49,0x73, 0x64,0x70,0x03,0x02,0x00,0x3c, 0x00, 0x14,0x6d,0x6f,0x73,0x71,0x70,0x75,0x62,0x2f,0x32,0x31,0x37,0x38,0x32,0x2d,0x75,0x62,0x75,0x6e,0x74,0x75};
    switch(event)
    {
        case ATINY_EV_CONNECTED:
            printf("now mqtt connect\n");
            mqtt_pack_con_opt_t options;
            memset(&options, 0, sizeof(options));
            options.clientID.cstring = "liteiot";
            options.MQTTVersion = 4;
            options.keepAliveInterval = 60;
            options.willFlag = 0;
			//nc->mgr->interface->ifuncs->send(nc, buf, 36);
		    atiny_mqtt_connect(nc, &options);
			//sleep(60);
            break;
        case ATINY_EV_MQTT_CONNACK:
            break;
        default:
            break;
    }
}


int main()
{
    atiny_manager_t mgr;
    atiny_connect_param_t param;
    param.proto_type = SOCK_STREAM;
    param.server_ip = "127.0.0.1";
    param.server_port = 1883;

    default_dev_info.ifuncs = &linux_sock;
    
    atiny_init(&mgr, &default_dev_info);

    atiny_connect_with_param(&mgr, ev_handler, param);
    for(;;)
    {
        atiny_poll(&mgr, 1000);
    }
}


