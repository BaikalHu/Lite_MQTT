#include "atiny.h"
#include "atiny_log.h"
#include "atiny_mqtt.h"

static int getNextPacketId(atiny_connection_t *nc)
{
    atiny_mqtt_proto_data_t *pd = (atiny_mqtt_proto_data_t *)nc->proto_data;
    return pd->next_packetid = (pd->next_packetid == MAX_PACKET_ID) ? 1 : pd->next_packetid + 1;
}


int atiny_mqtt_parser(atiny_buf_t *io, atiny_mqtt_msg_t *amm)
{
    MQTTHeader header = {0};

    unsigned char i = 0;
    unsigned int index = 1;
    int len = 0;
    const char *p, *end;
    int rem_len = 0;
    int multiplier = 1;
    const int MAX_NO_OF_REMAINING_LENGTH_BYTES = 4;

    header.byte = io->data[0];


    if(io->len < 2) return MQTTPACKET_BUFFER_TOO_SHORT;
    p = io->data + 1;
    while ((size_t)(p - io->data) < io->len)
    {
        i = *((const unsigned char *) p++);
        rem_len += (i & 0x7f) << 7 * len;
        len++;
        if (!(i & 0x80)) break;
        if (len > MAX_NO_OF_REMAINING_LENGTH_BYTES) return MQTTPACKET_READ_ERROR;
    }

    end = p + rem_len;
    if(i & 0x80 || rem_len > (io->len - (p - io->data)))
    {
        return MQTTPACKET_READ_ERROR;
    }

    amm->type = header.bits.type;
    amm->qos = header.bits.qos;

    switch(amm->type)
    {
        case PINGRESP:
            break;
        case CONNACK:
        case PUBACK:
            break;
        case SUBACK:
            {
                atiny_mqtt_suback_data_t data;
                data.grantedQoS= QOS0;
                int count = 0;
                unsigned short msgid;
                if (MQTTDeserialize_suback(&msgid, 1, &count, (int *)&data.grantedQoS, io->data, rem_len) == 1)
                {
                    if (data.grantedQoS != 0x80)
                        printf("~~~~~recv suback msgid:%d\n", msgid);
                    //rc = MQTTSetMessageHandler(c, topicFilter, messageHandler);
                }
            }
            break;
        case UNSUBACK:
            break;
        case PUBLISH:
            {
                MQTTString topicName;
                int intQoS;
                amm->payloadlen = 0; /* this is a size_t, but deserialize publish sets this as int */
                if (MQTTDeserialize_publish(&amm->dup, &intQoS, &amm->retained, &amm->id, &topicName,
                                        (unsigned char **)&amm->payload, (int *)&amm->payloadlen, io->data, rem_len) != 1)
                {
                    printf("deserialize pulish failed\n");
                }
                amm->qos = (enum QoS)intQoS;
    									/*
                deliverMessage(c, &topicName, &msg);
                if (msg.qos != QOS0)
                {
                    if (msg.qos == QOS1)
                    len = MQTTSerialize_ack(c->buf, c->buf_size, PUBACK, 0, msg.id);
                else if (msg.qos == QOS2)
                    len = MQTTSerialize_ack(c->buf, c->buf_size, PUBREC, 0, msg.id);
                if (len <= 0)
                    rc = FAILURE;
                else
                {
                    TimerInit(&send_timer);
                    TimerCountdownMS(&send_timer, 1000);
                    rc = sendPacket(c, len, &send_timer);
                }
                if (rc == FAILURE)
                    goto exit; // there was a problem
                    */
            }
            break;
    }

    amm->len = end - io->data;
    return amm->len;
}

void atiny_mqtt_event_handler(atiny_connection_t *nc, int event, void *event_data)
{
    int len;
    atiny_mqtt_msg_t amm;
    atiny_mqtt_proto_data_t *data = (atiny_mqtt_proto_data_t *)nc->proto_data;
    atiny_time_t now;
    atiny_buf_t *io = &nc->recv_buf;
    switch(event)
    {
        case ATINY_EV_RECV:
            while(1)
            {
                len = atiny_mqtt_parser(io, &amm);
                if(len <= 0)
                {
                    /* TODO */
                    break;
                }
                nc->user_handler(nc, ATINY_EV_MQTT_BASE + amm.type, &amm);
                memmove(nc->recv_buf.data, nc->recv_buf.data + len, nc->recv_buf.len - len);
                nc->recv_buf.len -= len;
            }
            break;
        case ATINY_EV_POLL:
            now = atiny_gettime_ms();
            printf("poll 0x%ld  0x%ld\n", now, data->last_time);
            if((now - data->last_time > data->keep_alive*1000) && (data->last_time > 0) && (data->keep_alive > 0))
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
    atiny_mqtt_proto_data_t *data;
    mqtt_pack_con_opt_t default_options = MQTTPacket_connectData_initializer;
    int len = 0;

    if (options == NULL)
        options = &default_options; /* set default options if none were supplied */

    data = (atiny_mqtt_proto_data_t *)nc->proto_data;
    data->keep_alive = options->keepAliveInterval;
    data->next_packetid = 1;

    if((len = MQTTSerialize_connect((nc->send_buf.data + nc->send_buf.len), (nc->send_buf.size - nc->send_buf.len), options)) <= 0)
        printf("ser connect error\n");
    nc->send_buf.len += len;
    data->last_time = atiny_gettime_ms();

    return rc;
}

int atiny_mqtt_ping(atiny_connection_t *nc)
{
    int len = 0;
    atiny_mqtt_proto_data_t *data;
    data = (atiny_mqtt_proto_data_t *)nc->proto_data;
    len = MQTTSerialize_pingreq((nc->send_buf.data + nc->send_buf.len), (nc->send_buf.size - nc->send_buf.len));
    if(len > 0)
        nc->send_buf.len += len;
    data->last_time = atiny_gettime_ms();

    return len;
}

int atiny_mqtt_publish(atiny_connection_t *nc, const char *topic_string, atiny_mqtt_msg_t *msg)
{
    int len = 0;

    MQTTString topic = MQTTString_initializer;
    topic.cstring = (char *)topic_string;
    atiny_mqtt_proto_data_t *data;
    data = (atiny_mqtt_proto_data_t *)nc->proto_data;
    len = MQTTSerialize_publish((nc->send_buf.data + nc->send_buf.len), (nc->send_buf.size - nc->send_buf.len), 0, msg->qos, msg->retained, msg->id,
                                topic, (unsigned char *)msg->payload, (int)msg->payloadlen);
    if(len > 0)
        nc->send_buf.len += len;
    data->last_time = atiny_gettime_ms();

    return len;
}

int atiny_mqtt_subscribe(atiny_connection_t *nc, const char *topics, QoS_e qos)
{
    int len = 0;
    MQTTString topic = MQTTString_initializer;
    topic.cstring = (char *)topics;
    atiny_mqtt_proto_data_t *data;
    data = (atiny_mqtt_proto_data_t *)nc->proto_data;

    len = MQTTSerialize_subscribe((nc->send_buf.data + nc->send_buf.len), (nc->send_buf.size - nc->send_buf.len), 0, getNextPacketId(nc), 1, &topic, (int *)&qos);
    if(len > 0)
        nc->send_buf.len += len;
    data->last_time = atiny_gettime_ms();

    return len;
}
