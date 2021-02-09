#include "mqtt_client.h"
#include "mqtt.h"
#include "osapi.h"
#include "c_types.h"
#include "osapi.h"
#include "mem.h"
#include "string.h"
#include "os_type.h"
#include "osapi.h"
#include "led_white.h"
#include "get_status.h"
#include "string.h"
#include "controller.h"

MQTT_Client mqttClient;
os_timer_t status_timer;
//const char poweroff = "poweroff";

void ICACHE_FLASH_ATTR connect_to_mqtt_server() {
	MQTT_InitConnection(&mqttClient, mqtt_host_address, mqtt_host_port,
			mqtt_security);
	MQTT_InitClient(&mqttClient, client_id_str, user_name, user_password, 5, 1);
	os_printf("connect to MQTT server...\n");
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);
	MQTT_Connect(&mqttClient);
}
void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	os_printf("MQTT: Connected\r\n");
	MQTT_Subscribe(client, client_id_str, 0);
	start_timer(client);
}

void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	os_timer_disarm(&status_timer);
	os_printf("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args) {
	MQTT_Client* client = (MQTT_Client*) args;
	os_printf("MQTT: Published\r\n");
}

void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len,
	const char *data, uint32_t data_len) {
	char *topicBuf = (char*) os_zalloc(topic_len + 1);
	char *dataBuf = (char*) os_zalloc(data_len + 1);

	MQTT_Client* client = (MQTT_Client*) args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	os_printf("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	if(strcmp(dataBuf, "power_off") == 0){
		os_printf("power_off\n");
		power_off();
	}else if(strcmp(dataBuf, "power_on") == 0){
		os_printf("power_on\n");
		power_on();
	}else if(strcmp(dataBuf, "power_reset") == 0){
		os_printf("power_reset\n");
		restart();
	}
	os_free(topicBuf);
	os_free(dataBuf);
}

void start_timer(MQTT_Client *client){
	//…Ë÷√∂® ±∆˜
	os_timer_disarm(&status_timer);
	os_timer_setfn(&status_timer, (os_timer_func_t *) timer_callback, client);
	os_timer_arm(&status_timer, report_timespan, 1);
}
void timer_callback(void *arg){
	MQTT_Client* client = (MQTT_Client*) arg;
	led_white_quick_flash();
	BOOL b = get_status();
	if(b){
		MQTT_Publish(client, "STATUS", "on", 0, 0);
	}else{
		MQTT_Publish(client, "STATUS", "off", 0, 0);
	}
}
