#include "os_type.h"
#include "check_wifi.h"
#include "smartconfig.h"
#include "user_interface.h"
#include "STA_INFO.h"
#include "osapi.h"
#include "led_green.h"

#define OS_Timer_WIFI_STATUS_CHECK_TIMESPAN	10000

os_timer_t OS_Timer_WIFI_STATUS_CHECK;
int errer_flag = 0;

void ICACHE_FLASH_ATTR init_wifi_status_checker() {
	os_timer_disarm(&OS_Timer_WIFI_STATUS_CHECK);	// 关闭定时器
	os_timer_setfn(&OS_Timer_WIFI_STATUS_CHECK,
			(os_timer_func_t *) OS_Timer_WIFI_STATUS_CALLBACK, NULL);	//设置定时器
	os_timer_arm(&OS_Timer_WIFI_STATUS_CHECK,
			OS_Timer_WIFI_STATUS_CHECK_TIMESPAN, true);	//启动定时器
}

void ICACHE_FLASH_ATTR OS_Timer_WIFI_STATUS_CALLBACK() {
	uint8 wifi_status = wifi_station_get_connect_status();
//	os_printf("wifi status---");
	if (wifi_status == STATION_IDLE) {
		os_printf("wifi status---waitting...\n");
	} else if (wifi_status == STATION_CONNECTING) {
		os_printf("wifi status---connecting...\n");
	} else if (wifi_status == STATION_WRONG_PASSWORD) {
		os_printf("wifi status---wrong password\n");
		wifi_status_error();
	} else if (wifi_status == STATION_NO_AP_FOUND) {
		os_printf("wifi status---wifi signal not found\n");
		wifi_status_error();
	} else if (wifi_status == STATION_CONNECT_FAIL) {
		os_printf("wifi status---connect failed\n");
		wifi_status_error();
	} else if (wifi_status == STATION_GOT_IP) {
//		os_printf("wifi status---OK\n");
		led_green_on();
	}
}

void ICACHE_FLASH_ATTR wifi_status_error() {
	errer_flag++;
	led_green_off();
	if (errer_flag == 3) {
		os_printf("system restart\n");
		system_restart();
	}
}
