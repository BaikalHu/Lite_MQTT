#include <string.h>

#include "mqtt_packet.h"

static int mqtt_encode_len(unsigned char *buf, int len)
{
    int rc = 0;

    do
    {
        unsigned char d = len & (MQTT_PACKET_LEN_MASK - 1);
        len >>= MQTT_PACKET_LEN_BIT_NUM;
		// if there are more data to encode, set the top bit of this byte
        if (len > 0)
            d |= MQTT_PACKET_LEN_MASK;
        buf[rc++] = d;
    } while (len > 0);

    return rc;
}

static int mqtt_encode_num(unsigned char *buf, unsigned short num)
{
    if(buf)
    {
        buf[0] = num >> 8;
        buf[1] = num & 0xFF;
    }
    
    return MQTT_STRING_LEN;
}

static int mqtt_encode_string(unsigned char *buf, const unsigned char *str)
{
    int str_len = 0;
	int len = 0;

	str_len = strlen(str);
	len = mqtt_encode_num(buf, str_len);
	memcpy(buf + len, str, str_len);

    return  (len + str_len);
}


int mqtt_encode_fixhead(unsigned char *buf, unsigned char type, unsigned char dup, 
                                unsigned char qos, unsigned char retain, int remaining_len)
{
    int len = 0;

    mqtt_fix_head_t *head = (mqtt_fix_head_t *)buf;

	head->mqtt_first_byte_u.bits.type = type;
	head->mqtt_first_byte_u.bits.dup = dup;
	head->mqtt_first_byte_u.bits.qos = qos;
    head->mqtt_first_byte_u.bits.retain = retain;
    len = mqtt_encode_len( head->remaining_len, remaining_len);

	return (len+1);
}

int mqtt_encode_connect(unsigned char *buf, int buf_len, mqtt_connect_opt_t *options)
{
    int len = 0;
    int remaining_len = 0;
    unsigned char *vhead_buf;
    unsigned char *payload_buf;

    // add connect variable header size
    remaining_len += sizeof(mqtt_connect_head_t); 

    // add payload size
    remaining_len += (int)strlen(options->connect_payload.client_id) + MQTT_STRING_LEN;
    if(options->connect_head.mqtt_connect_flag_u.bits.will_flag 
		&& options->connect_payload.will_topic
		&& options->connect_payload.will_msg)
    {
        remaining_len += (int)strlen(options->connect_payload.will_topic) + MQTT_STRING_LEN;
        remaining_len += (int)strlen(options->connect_payload.will_msg) + MQTT_STRING_LEN;
    }

    if(options->connect_head.mqtt_connect_flag_u.bits.user_name_flag && options->connect_payload.user_name)
        remaining_len += (int)strlen(options->connect_payload.user_name) + MQTT_STRING_LEN;

    if(options->connect_head.mqtt_connect_flag_u.bits.psd_flag && options->connect_payload.password)
        remaining_len += (int)strlen(options->connect_payload.password) + MQTT_STRING_LEN;

    /* Encode fix header */
    len = mqtt_encode_fixhead(buf, MQTT_PACKET_TYPE_CONNECT, 0, 0, 0, remaining_len);
    if((len + remaining_len) > buf_len)
        return -1;

	/* check variable header and payload */
	// TODO
	/* Encode variable header */
    vhead_buf = buf + len;
	memcpy(vhead_buf, &(options->connect_head), sizeof(options->connect_head));

	/* Encode payload*/
	payload_buf = vhead_buf + sizeof(options->connect_head);

	payload_buf += mqtt_encode_string(payload_buf, options->connect_payload.client_id);
	
    if(options->connect_head.mqtt_connect_flag_u.bits.will_flag 
		&& options->connect_payload.will_topic
		&& options->connect_payload.will_msg)
    {
        payload_buf += mqtt_encode_string(payload_buf, options->connect_payload.will_topic);
		payload_buf += mqtt_encode_string(payload_buf, options->connect_payload.will_msg);
    }

    if(options->connect_head.mqtt_connect_flag_u.bits.user_name_flag && options->connect_payload.user_name)
		payload_buf += mqtt_encode_string(payload_buf, options->connect_payload.user_name);
	
	if(options->connect_head.mqtt_connect_flag_u.bits.psd_flag && options->connect_payload.password)
		payload_buf += mqtt_encode_string(payload_buf, options->connect_payload.password);

	return (len + remaining_len);
}

int mqtt_encode_publish(unsigned char *buf, int buf_len, mqtt_publish_opt_t *options)
{
    int len = 0;
    int remaining_len = 0;
    unsigned char *vhead_buf;
    unsigned char *payload_buf;

    // add connect variable header size
    if(options->publish_head.topic)
        remaining_len += (int)strlen(options->publish_head.topic) + MQTT_STRING_LEN;

	remaining_len += sizeof(options->publish_head.packet_id);

	if(options->publish_payload.msg)
		remaining_len += (int)strlen(options->publish_payload.msg);
    /* Encode fix header */
    len = mqtt_encode_fixhead(buf, MQTT_PACKET_TYPE_PUBLISH, options->dup, 
                              options->qos, options->retain, remaining_len);

	/* Encode variable header */
    vhead_buf = buf + len;
    vhead_buf += mqtt_encode_string(vhead_buf, options->publish_head.topic);
	if(options->qos > 0)
	    vhead_buf += mqtt_encode_num(vhead_buf, options->publish_head.packet_id);

	/* Encode payload*/
	payload_buf = vhead_buf;
	memcpy(payload_buf, options->publish_payload.msg, strlen(options->publish_payload.msg));

	return (len + remaining_len);
}

int mqtt_encode_subscribe(unsigned char *buf, int buf_len, mqtt_subscribe_opt_t *options)
{
    int len = 0;
    int remaining_len = 0;
    unsigned char *vhead_buf;
    unsigned char *payload_buf;

    remaining_len += sizeof(options->subscribe_head);

	for(int i = 0; i < options->subscribe_payload.count; i++)
    {
        printf("~~~~~%s\n", (options->subscribe_payload.topic[i]));
        remaining_len += MQTT_STRING_LEN + MQTT_QOS_LEN + (int)strlen(options->subscribe_payload.topic[i]);
	}
	
    /* Encode fix header */
    len = mqtt_encode_fixhead(buf, MQTT_PACKET_TYPE_SUBSCRIBE, 0, 0, 0, remaining_len);
	/* Encode variable header */
	vhead_buf = buf + len;
	vhead_buf += mqtt_encode_num(vhead_buf, options->subscribe_head.packet_id);

	/* Encode payload*/
	payload_buf = vhead_buf;
	for(int i = 0; i < options->subscribe_payload.count; i++)
    {
        payload_buf += mqtt_encode_string(payload_buf, options->subscribe_payload.topic[i]);
		*payload_buf = options->subscribe_payload.qoss[i];
		//printf("p%d\n", *payload_buf);
		payload_buf += 1;
	}

    return (len + remaining_len);
}

int mqtt_decode_publish(unsigned char *buf, int buf_len, mqtt_publish_opt_t *options)
{



}

