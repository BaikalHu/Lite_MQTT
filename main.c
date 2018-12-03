#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "atiny_log.h"
#include "atiny.h"
#include "atiny_mqtt.h"

static atiny_device_info_t default_dev_info;
extern atiny_if_funcs_t linux_sock;

void ev_handler(atiny_connection_t *nc, int event, void *event_data)
{
    printf("ev_handler in main\n");
	atiny_mqtt_msg_t *amm = (atiny_mqtt_msg_t *)event_data;
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
			nc->proto_handler = atiny_mqtt_event_handler;
			nc->proto_data = atiny_malloc(sizeof(atiny_mqtt_proto_data_t));
		    atiny_mqtt_connect(nc, &options);
            break;
        case ATINY_EV_MQTT_CONNACK:
			printf("connect succuss~~~~~~~\n");
			atiny_mqtt_msg_t msg;
		    msg.dup = 0;
			msg.id = 0x1000;
			msg.qos = 1;
			msg.retained = 0;
			msg.payload = "hello";
			msg.payloadlen = 5;
			atiny_mqtt_publish(nc,"abc",&msg);
			atiny_mqtt_subscribe(nc,"sub",1);
			atiny_mqtt_subscribe(nc,"sub1",1);
            break;
		case ATINY_EV_MQTT_PUBLISH:
            printf("recv pushlish %s\n", amm->payload);
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


