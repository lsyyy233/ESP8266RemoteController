#include "wifi_connect.h"
#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "smartconfig.h"
#include "airkiss.h"
#include "c_types.h"

os_timer_t OS_Timer_IP;
connect_success_callback *connect_success_callback_func;

void ICACHE_FLASH_ATTR connect(connect_success_callback *func) {
	connect_success_callback_func = func;
	struct station_config STA_INFO;
	os_memset(&STA_INFO, 0, sizeof(struct station_config));
	spi_flash_read(STA_INFO_ADDR * 4096, (uint32 *) &STA_INFO, 96);	// 读出【STA参数】(SSID/PASS)
	STA_INFO.ssid[31] = 0;		// SSID最后添'\0'
	STA_INFO.password[63] = 0;	// APSS最后添'\0'
	os_printf("STA_INFO.ssid=%s\r\nSTA_INFO.password=%s\n", STA_INFO.ssid, STA_INFO.password);
	os_printf("-------------------------------\n");
	//连接wifi
	wifi_set_opmode(0x01);
	wifi_station_set_config(&STA_INFO);
	wifi_station_connect();

	os_timer_disarm(&OS_Timer_IP);	// 关闭定时器
	os_timer_setfn(&OS_Timer_IP, (os_timer_func_t *) OS_Timer_IP_cb, NULL);	//设置定时器
	os_timer_arm(&OS_Timer_IP, 333, true);	//启动定时器
}

/*
 * 定时器回调函数，检查wifi状态
 */
void ICACHE_FLASH_ATTR OS_Timer_IP_cb(void) {
	uint8 wifi_status = wifi_station_get_connect_status();
	switch (wifi_status) {
	case STATION_IDLE:	//等待
		os_printf("waitting...\n");
		break;
	case STATION_CONNECTING:	//正在连接
		os_printf("connecting...\n");
		break;
	case STATION_WRONG_PASSWORD:	//密码错误
		os_timer_disarm(&OS_Timer_IP);
		os_printf("wrong password!!!\n");
		start_smartconfig();
		break;
	case STATION_NO_AP_FOUND:	//没有找到wifi
		os_timer_disarm(&OS_Timer_IP);
		os_printf("wifi signal not found!!!\n");
		start_smartconfig();
		break;
	case STATION_CONNECT_FAIL:	//连接失败
		os_timer_disarm(&OS_Timer_IP);
		os_printf("connect failed!!!\n");
		start_smartconfig();
		break;
	case STATION_GOT_IP:	//成功获取IP
		os_timer_disarm(&OS_Timer_IP);
		os_printf("connect success!!!\n");
		connect_success_callback_func();
		break;
	}
}

void ICACHE_FLASH_ATTR start_smartconfig() {
	os_printf("start smartconfig...\n");
	smartconfig_set_type(SC_TYPE_AIRKISS); //SC_TYPE_ESPTOUCH,SC_TYPE_AIRKISS,SC_TYPE_ESPTOUCH_AIRKISS
	wifi_set_opmode(STATION_MODE);
	smartconfig_start(smartconfig_done);
	os_printf("Waitting for configration\n");
}

/*
 *微信配网完成回调函数
 */
void ICACHE_FLASH_ATTR smartconfig_done(sc_status status, void *pdata) {
	//os_printf("airkiss_version = %s\n", *(smartconfig_get_version()));
	switch (status) {
	case SC_STATUS_WAIT:
		os_printf("SC_STATUS_WAIT\n");
		break;
	case SC_STATUS_FIND_CHANNEL:
		os_printf("SC_STATUS_FIND_CHANNEL\n");
		break;
	case SC_STATUS_GETTING_SSID_PSWD:
		os_printf("SC_STATUS_GETTING_SSID_PSWD\n");
		sc_type *type = pdata;
		if (*type == SC_TYPE_ESPTOUCH) {
			os_printf("SC_TYPE:SC_TYPE_ESPTOUCH\n");
		} else {
			os_printf("SC_TYPE:SC_TYPE_AIRKISS\n");
		}
		break;
	case SC_STATUS_LINK:
		os_printf("SC_STATUS_LINK\n");
		struct station_config *sta_conf = pdata;
		//保存STA信息
		spi_flash_erase_sector(STA_INFO_ADDR);
		spi_flash_write(STA_INFO_ADDR * 4096, (uint32 *) sta_conf, 128);

		wifi_station_set_config(sta_conf);
		wifi_station_disconnect();
		wifi_station_connect();
		break;
	case SC_STATUS_LINK_OVER:
		os_printf("SC_STATUS_LINK_OVER\n");
		if (pdata != NULL) {
			//SC_TYPE_ESPTOUCH
			uint8 phone_ip[4] = { 0 };

			os_memcpy(phone_ip, (uint8*) pdata, 4);
			os_printf("Phone ip: %d.%d.%d.%d\n", phone_ip[0], phone_ip[1],
					phone_ip[2], phone_ip[3]);
		} else {
			//SC_TYPE_AIRKISS - support airkiss v2.0
			//airkiss_start_discover();
		}
		smartconfig_stop();
		connect_success_callback_func();
		break;
	}
}
