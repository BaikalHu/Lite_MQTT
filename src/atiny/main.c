#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "atiny_log.h"
#include "atiny_adapter.h"
#include "atiny.h"
#include "atiny_mqtt.h"
#include "atiny_config.h"
#include "mqtt_packet.h"


static atiny_device_info_t default_dev_info;
extern atiny_if_funcs_t linux_sock;

void ev_handler(atiny_connection_t *nc, int event, void *event_data)
{
    ATINY_LOG(LOG_DEBUG, "ev_handler in main");
    atiny_mqtt_msg_t *amm = (atiny_mqtt_msg_t *)event_data;
    switch(event)
    {
        case ATINY_EV_CONNECTED:
            {
                ATINY_LOG(LOG_DEBUG, "now mqtt connect");
                mqtt_connect_opt_t options;

				atiny_register_proto(nc, atiny_mqtt_event_handler);
                memset(&options, 0, sizeof(options));
                options.connect_head = (mqtt_connect_head_t)MQTT_CONNECT_HEAD_INIT;
                options.connect_head.keep_alive = 60;
                options.connect_payload.client_id = "LiteIOT";
                options.connect_head.mqtt_connect_flag_u.bits.user_name_flag = 1;
                options.connect_head.mqtt_connect_flag_u.bits.psd_flag = 1;
                options.connect_payload.user_name = "LiteIOT";
                options.connect_payload.password = "123456";
                nc->proto_data = (void *)atiny_malloc((size_t)sizeof(atiny_mqtt_proto_data_t));
                atiny_mqtt_connect(nc, &options);
            }
            break;
        case ATINY_EV_MQTT_CONNACK:
            {
                ATINY_LOG(LOG_DEBUG, "mqtt connect succuss");
                mqtt_subscribe_opt_t sub_options;
                sub_options.subscribe_payload.count = 5;
                sub_options.subscribe_payload.topic = (char **)malloc(5);
                //sub_options.subscribe_payload.topic[0] = "/bj/dcf/33/p";
                sub_options.subscribe_payload.topic[0] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/properties/result";
                sub_options.subscribe_payload.topic[1] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/twins/result";
                sub_options.subscribe_payload.topic[2] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/properties/todevice";
                sub_options.subscribe_payload.topic[3] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/twins/expected";
                sub_options.subscribe_payload.topic[4] = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/messages/todevice/#";
                //sub_options.subscribe_payload.qoss = (unsigned char *)malloc(2);
                sub_options.subscribe_payload.qoss[0] = 0;
                sub_options.subscribe_payload.qoss[1] = 0;
                sub_options.subscribe_payload.qoss[2] = 0;
                sub_options.subscribe_payload.qoss[3] = 0;
                sub_options.subscribe_payload.qoss[4] = 0;
                atiny_mqtt_subscribe(nc, &sub_options);

                mqtt_publish_opt_t options;
                options.publish_head.topic = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/properties";
                options.publish_payload.msg = "hello";
                options.dup = 0;
                options.qos = 1;
                options.retain = 0;
                atiny_mqtt_publish(nc, &options);

                options.publish_head.topic = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/properties/tocloud";
                options.publish_payload.msg = "hello1";
                options.dup = 0;
                options.qos = 1;
                options.retain = 0;
                atiny_mqtt_publish(nc, &options);

                options.publish_head.topic = AGENT_TINY_PROJECT_ID"/device/"AGENT_TINY_DEVICE_ID"/twins";
                options.publish_payload.msg = "hello2";
                options.dup = 0;
                options.qos = 1;
                options.retain = 0;
                atiny_mqtt_publish(nc, &options);
            }
            break;
        case ATINY_EV_MQTT_PUBLISH:
            {
                ATINY_LOG(LOG_INFO, "recv pushlish %s", (char *)amm->payload);
                int len = 0;
                mqtt_puback_opt_t options;

                if (amm->qos != QOS0)
                {
                    if (amm->qos == QOS1)
                    {
                        options.type = MQTT_PACKET_TYPE_PUBACK;
                        len = atiny_mqtt_puback(nc, &options);
                    }
                    else if (amm->qos == QOS2)
                    {
                        options.type = MQTT_PACKET_TYPE_PUBREC;
                        len = atiny_mqtt_puback(nc, &options);
                    }
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
    param.server_ip = SERVER_IP;
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
