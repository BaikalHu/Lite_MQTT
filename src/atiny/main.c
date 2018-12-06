#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "atiny_log.h"
#include "atiny.h"
#include "atiny_mqtt.h"
#include "atiny_config.h"
#include "mqtt_packet.h"


static atiny_device_info_t default_dev_info;
extern atiny_if_funcs_t linux_sock;

void ev_handler(atiny_connection_t *nc, int event, void *event_data)
{
    printf("ev_handler in main\n");
	atiny_mqtt_msg_t *amm = (atiny_mqtt_msg_t *)event_data;
    switch(event)
    {
        case ATINY_EV_CONNECTED:
        {
            printf("now mqtt connect\n");
            mqtt_connect_opt_t options;
            memset(&options, 0, sizeof(options));
			options.connect_head = (mqtt_connect_head_t)MQTT_CONNECT_HEAD_INIT;
			options.connect_head.keep_alive = 60;
			options.connect_payload.client_id = "LiteIOT";
            nc->proto_handler = atiny_mqtt_event_handler;
            nc->proto_data = atiny_malloc(sizeof(atiny_mqtt_proto_data_t));
            atiny_mqtt_connect(nc, &options);
        }
        break;
        case ATINY_EV_MQTT_CONNACK:
        {
            printf("connect succuss~~~~~~~\n");
            mqtt_publish_opt_t options;
		    options.publish_head.topic = "abc";
			options.publish_payload.msg = "hello world";
			options.dup = 0;
			options.qos = 1;
			options.retain = 0;
            atiny_mqtt_publish(nc, &options);

			mqtt_subscribe_opt_t sub_options;
			sub_options.subscribe_payload.count = 2;
			sub_options.subscribe_payload.topic = (char **)malloc(2);
			sub_options.subscribe_payload.topic[0] = (char *)malloc(4);  memcpy(&sub_options.subscribe_payload.topic[0][0], "sub1", 4); printf("%s\n",sub_options.subscribe_payload.topic[0] );
			sub_options.subscribe_payload.topic[1] = (char *)malloc(4);  memcpy(&sub_options.subscribe_payload.topic[1][0], "sub2", 4);
			//sub_options.subscribe_payload.qoss = (unsigned char *)malloc(2);
			sub_options.subscribe_payload.qoss[0] = 0;
			sub_options.subscribe_payload.qoss[1] = 0; printf("debug1\n");
            atiny_mqtt_subscribe(nc, &sub_options);
        }
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
	param.server_port = SERVER_PORT;
#ifdef WITH_DTLS
    param.ssl_param.ca.server_name = server_name;
    param.ssl_param.ca.ca_cert = mqtt_test_cas_pem;
    param.ssl_param.ca.ca_cert_len = mqtt_test_cas_pem_len;
    param.ssl_param.ca.client_cert = mqtt_test_cli_pem;
    param.ssl_param.ca.client_cert_len = mqtt_test_cli_pem_len;
    param.ssl_param.ca.client_key = mqtt_test_cli_key;
    param.ssl_param.ca.client_key_len = mqtt_test_cli_key_len;
#endif

    default_dev_info.ifuncs = &linux_sock;
    
    atiny_init(&mgr, &default_dev_info);

    atiny_connect_with_param(&mgr, ev_handler, param);
    for(;;)
    {
        atiny_poll(&mgr, 1000);
    }
}
