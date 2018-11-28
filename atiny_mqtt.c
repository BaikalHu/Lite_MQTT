#include "atiny.h"
#include "atiny_log.h"
#include "atiny_mqtt.h"

int atiny_mqtt_parser(atiny_buf_t *io, atiny_mqtt_msg_t *amm)
{


}

static atiny_mqtt_event_handler(atiny_connection_t *nc, int event, void *event_data)
{
    atiny_buf_t *io = &nc->recv_buf;
    switch(event)
    {
        case ATINY_EV_RECV:
            //atiny_mqtt_parser();
            break;
        case ATINY_EV_POLL:
            if(1)
            {
                ATINY_LOG(LOG_DEBUG, "Send ping request");
				atiny_mqtt_ping(nc);
			}
            break;
    }
}

int atiny_mqtt_connect(atiny_connection_t *nc, mqtt_pack_con_opt_t *options)
{
    int rc = -1;
    mqtt_pack_con_opt_t default_options = MQTTPacket_connectData_initializer;
    int len = 0;

    if (options == 0)
        options = &default_options; /* set default options if none were supplied */

	if((len = MQTTSerialize_connect((nc->send_buf.data + nc->send_buf.len), (nc->send_buf.size - nc->send_buf.len), options)) <= 0)
        printf("ser connect error\n");
	nc->send_buf.len += len;
    return rc;
}

void atiny_mqtt_ping(atiny_connection_t *nc)
{
    int rc = -1;
	int len = 0;
    len = MQTTSerialize_pingreq((nc->send_buf.data + nc->send_buf.len), (nc->send_buf.size - nc->send_buf.len));
    nc->send_buf.len += len;
}
