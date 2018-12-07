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
                options.connect_payload.client_id = "admin&htfy";
				options.connect_head.mqtt_connect_flag_u.bits.user_name_flag = 1;
				options.connect_head.mqtt_connect_flag_u.bits.psd_flag = 1;
				options.connect_payload.user_name = "admin&htfy";
				options.connect_payload.password = "123456";
                nc->proto_handler = atiny_mqtt_event_handler;
                nc->proto_data = atiny_malloc(sizeof(atiny_mqtt_proto_data_t));
                atiny_mqtt_connect(nc, &options);
            }
            break;
        case ATINY_EV_MQTT_CONNACK:
            {
                printf("connect succuss~~~~~~~\n");
                mqtt_publish_opt_t options;
                options.publish_head.topic = "/bj/dcf/33/s";
                options.publish_payload.msg = "hello";
                options.dup = 0;
                options.qos = 1;
                options.retain = 0;
                atiny_mqtt_publish(nc, &options);

				printf("publish~~~~~~~\n");
                mqtt_subscribe_opt_t sub_options;
                sub_options.subscribe_payload.count = 1;
                sub_options.subscribe_payload.topic = (char **)malloc(1);
				sub_options.subscribe_payload.topic[0] = "/bj/dcf/33/p";
                //sub_options.subscribe_payload.topic[0] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/properties/result";
                //sub_options.subscribe_payload.topic[1] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/twins/result";
				//sub_options.subscribe_payload.topic[2] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/properties/todevice";
				//	sub_options.subscribe_payload.topic[3] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/twins/expected";
				//	sub_options.subscribe_payload.topic[4] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/messages/todevice/#";
                //sub_options.subscribe_payload.qoss = (unsigned char *)malloc(2);
                sub_options.subscribe_payload.qoss[0] = 0;
                //sub_options.subscribe_payload.qoss[1] = 0;
				//sub_options.subscribe_payload.qoss[2] = 0;
				//sub_options.subscribe_payload.qoss[3] = 0;
				//sub_options.subscribe_payload.qoss[4] = 0;
                atiny_mqtt_subscribe(nc, &sub_options);
            }
            break;
        case ATINY_EV_MQTT_PUBLISH:
            {
                printf("recv pushlish %s\n", amm->payload);
                int len = 0;

                if (amm->qos != QOS0)
                {
                    if (amm->qos == QOS1)
                        ;//len = MQTTSerialize_ack((nc->send_buf.data + nc->send_buf.len), c->buf_size, PUBACK, 0, amm->id);
                    else if (amm->qos == QOS2)
                        ;//len = MQTTSerialize_ack(c->buf, c->buf_size, PUBREC, 0, msg.id);
                    if (len <= 0)
                        printf("error\n");
                }
           }
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
    param.server_ip = "118.190.136.167";//"122.112.225.88";//"127.0.0.1";
	param.server_port = 1883;//SERVER_PORT;
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
