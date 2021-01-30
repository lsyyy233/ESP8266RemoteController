#include "wifi_status.h"
#include "gpio.h"
#include "osapi.h"
#include "c_types.h"

void ICACHE_FLASH_ATTR init_wifi_led(){
	PIN_FUNC_SELECT(WIFI_STATUS_PIN_NAME,	WIFI_STATUS_PIN_FUNC);
	os_delay_us(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WIFI_STATUS_GPIO_NUM),0);
	os_delay_us(1000);
}

void ICACHE_FLASH_ATTR set_wifi_led_ok(){
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WIFI_STATUS_GPIO_NUM),1);
	os_delay_us(1000);
}
void ICACHE_FLASH_ATTR set_wifi_led_error(){
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WIFI_STATUS_GPIO_NUM),0);
	os_delay_us(1000);
}
