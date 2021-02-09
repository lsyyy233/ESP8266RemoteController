#define mqtt_host_address	"192.168.50.240"
#define mqtt_host_port		5002
#define mqtt_security		0
#define client_name			"esxi_31"
#define user_name			"admin"
#define user_password		"password"
#define client_id_str		"19C31AC7-656E-F6CC-814E-A716C0A2BF82"
#define report_timespan		3000

#include "mqtt.h"

#include "c_types.h"

void connect_to_mqtt_server();
void mqttConnectedCb(uint32_t *args);
void mqttDisconnectedCb(uint32_t *args);
void mqttPublishedCb(uint32_t *args);
void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len);
void start_timer(MQTT_Client* client);
void timer_callback(void *arg);
