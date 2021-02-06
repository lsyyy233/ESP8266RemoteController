#include "sntp_init.h"
#include "sntp.h"
#include "ets_sys.h"
#include "osapi.h"
#include "eagle_soc.h"
#include "gpio.h"
#include "mqtt_client.h"
#include "led/led_white.h"

static ETSTimer sntp_timer;

void ICACHE_FLASH_ATTR start_sntp() {
	sntp_setservername(0, SNTP_SERVER_0);
	sntp_setservername(1, SNTP_SERVER_1);
	sntp_setservername(2, SNTP_SERVER_2);
	sntp_init();
	os_printf("start sntp\n");
	os_delay_us(3000000);

	// 设置SNTP定时器[sntp_timer]
	//-----------------------------------------------------------
	os_timer_disarm(&sntp_timer);
	os_timer_setfn(&sntp_timer, (os_timer_func_t *) sntpfn, NULL);
	os_timer_arm(&sntp_timer, 1000, 1);		// 1s定时
}

/*
 * SNTP定时函数：获取当前网络时间
 */
void ICACHE_FLASH_ATTR sntpfn() {
	u32_t ts = 0;

	ts = sntp_get_current_timestamp();		// 获取当前的偏移时间
	if (ts == 0){//网络时间获取失败
		os_printf("did not get a valid time from sntp server\n");
	} else{ //(ts != 0)网络时间获取成功
		os_printf("current time : %s", sntp_get_real_time(ts));	// 获取真实时间
		os_timer_disarm(&sntp_timer);	// 关闭SNTP定时器
		led_white_off();
		connect_to_mqtt_server();// 开始MQTT连接
	}
}
