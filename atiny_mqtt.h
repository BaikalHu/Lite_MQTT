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
    QoS_e qos;
    unsigned char retained;
    unsigned char dup;
    unsigned short id;
    void *payload;
    size_t playloadlen;
} atiny_mqtt_msg_t;

int atiny_mqtt_connect(atiny_connection_t *nc, mqtt_pack_con_opt_t *options);



