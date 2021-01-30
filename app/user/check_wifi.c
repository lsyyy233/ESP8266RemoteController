#include "os_type.h"
#include "check_wifi.h"
#include "smartconfig.h"
#include "user_interface.h"
#include "STA_INFO.h"
#include "osapi.h"

#define OS_Timer_WIFI_STATUS_CHECK_TIMESPAN	3000

os_timer_t OS_Timer_WIFI_STATUS_CHECK;
int errer_flag = 0;

void ICACHE_FLASH_ATTR init_wifi_status_checker(){
	os_timer_disarm(&OS_Timer_WIFI_STATUS_CHECK);	// 关闭定时器
	os_timer_setfn(&OS_Timer_WIFI_STATUS_CHECK, (os_timer_func_t *) OS_Timer_WIFI_STATUS_CALLBACK, NULL);	//设置定时器
	os_timer_arm(&OS_Timer_WIFI_STATUS_CHECK, OS_Timer_WIFI_STATUS_CHECK_TIMESPAN, true);	//启动定时器
}

void ICACHE_FLASH_ATTR OS_Timer_WIFI_STATUS_CALLBACK(){
	uint8 wifi_status = wifi_station_get_connect_status();
	os_printf("wifi status---");
	if(wifi_status == STATION_IDLE){
		os_printf("waitting...\n");
	}else if(wifi_status == STATION_CONNECTING){
		os_printf("connecting...\n");
	}else if(wifi_status == STATION_WRONG_PASSWORD){
		os_printf("wrong password\n");
		errer_flag ++ ;
	}else if(wifi_status == STATION_NO_AP_FOUND){
		errer_flag ++ ;
		os_printf("wifi signal not found\n");
	}else if(wifi_status == STATION_CONNECT_FAIL){
		os_printf("connect failed\n");
		errer_flag ++ ;
	}else if(wifi_status == STATION_GOT_IP){
		os_printf("OK\n");
	}
	if(errer_flag == 3){
		os_printf("system restart\n");
		system_restart();
	}
}
