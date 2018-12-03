#include "MQTTPacket.h"

#include "MQTTConnect.h"

#define ATINY_EV_MQTT_BASE    0x1000
#define ATINY_EV_MQTT_CONNECT          (ATINY_EV_MQTT_BASE + CONNECT)
#define ATINY_EV_MQTT_CONNACK          (ATINY_EV_MQTT_BASE + CONNACK)
#define ATINY_EV_MQTT_PUBLISH          (ATINY_EV_MQTT_BASE + PUBLISH)
#define ATINY_EV_MQTT_PUBACK           (ATINY_EV_MQTT_BASE + PUBACK)
#define ATINY_EV_MQTT_PUBREC           (ATINY_EV_MQTT_BASE + PUBREC)
#define ATINY_EV_MQTT_PUBREL           (ATINY_EV_MQTT_BASE + PUBREL)
#define ATINY_EV_MQTT_PUBCOMP          (ATINY_EV_MQTT_BASE + PUBCOMP)
#define ATINY_EV_MQTT_SUBSCRIBE        (ATINY_EV_MQTT_BASE + SUBSCRIBE)
#define ATINY_EV_MQTT_SUBACK           (ATINY_EV_MQTT_BASE + SUBACK)
#define ATINY_EV_MQTT_UNSUBSCRIBE      (ATINY_EV_MQTT_BASE + UNSUBSCRIBE)
#define ATINY_EV_MQTT_UNSUBACK         (ATINY_EV_MQTT_BASE + UNSUBACK)
#define ATINY_EV_MQTT_PINGREQ          (ATINY_EV_MQTT_BASE + PINGREQ)
#define ATINY_EV_MQTT_PINGRESP         (ATINY_EV_MQTT_BASE + PINGRESP)
#define ATINY_EV_MQTT_DISCONNECT       (ATINY_EV_MQTT_BASE + DISCONNECT)

#define MAX_MESSAGE_HANDLERS (10)

typedef MQTTPacket_connectData mqtt_pack_con_opt_t;

#define MAX_PACKET_ID 0xFFFF

typedef enum QoS 
{
    QOS0, 
    QOS1, 
    QOS2
} QoS_e;

typedef struct atiny_mqtt_msg
{
    unsigned char type;
    QoS_e qos;
    unsigned int len;
    unsigned char retained;
    unsigned char dup;
    unsigned short id;
    void *payload;
    size_t payloadlen;
} atiny_mqtt_msg_t;

typedef struct atiny_mqtt_msg_data
{
    atiny_mqtt_msg_t* message;
    const char* topic;
} atiny_mqtt_msg_data_t;

typedef struct atiny_mqtt_suback_data
{
    QoS_e grantedQoS;
} atiny_mqtt_suback_data_t;


typedef void (*messageHandler)(atiny_mqtt_msg_data_t*);

typedef struct atiny_mqtt_proto_data
{
    unsigned short keep_alive;
    atiny_time_t last_time;
    unsigned int next_packetid;
    struct MessageHandlers
    {
        const char* topicFilter;
        messageHandler fp;
    } messageHandlers[MAX_MESSAGE_HANDLERS];      /* Message handlers are indexed by subscription topic */
} atiny_mqtt_proto_data_t;



int atiny_mqtt_connect(atiny_connection_t *nc, mqtt_pack_con_opt_t *options);
int atiny_mqtt_ping(atiny_connection_t *nc);

void atiny_mqtt_event_handler(atiny_connection_t *nc, int event, void *event_data);



