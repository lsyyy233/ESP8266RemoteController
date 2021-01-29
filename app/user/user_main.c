#include "sntp.h"
#include "ets_sys.h"
#include "osapi.h"
#include "ip_addr.h"
#include "espconn.h"
#include "mem.h"
#include "driver/uart.h"
#include "user_interface.h"
#include "smartconfig.h"
#include "airkiss.h"
#include "driver/hw_timer.h"
#include "wifi_connect.h"
#include "c_types.h"
#include "sntp_init.h"

#define STA_INFO_ADDR	0x80

#define SPI_FLASH_SIZE_MAP	6
#define SYSTEM_PARTITION_OTA_SIZE							0x07f000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000

void connect_success(void);

uint8_t lan_buf[200];
uint16_t lan_buf_len;
uint8 udp_sent_cnt = 0;
os_timer_t OS_Timer_IP;

//type,addr,size
static const partition_item_t at_partition_table[] = {
		{SYSTEM_PARTITION_BOOTLOADER, 		0x0, 										0x1000 },
		{SYSTEM_PARTITION_OTA_1,			0x1000, 									SYSTEM_PARTITION_OTA_SIZE },
		{SYSTEM_PARTITION_OTA_2,			SYSTEM_PARTITION_OTA_2_ADDR, 				SYSTEM_PARTITION_OTA_SIZE },
		{SYSTEM_PARTITION_RF_CAL, 			SYSTEM_PARTITION_RF_CAL_ADDR, 				0x1000 },
		{SYSTEM_PARTITION_PHY_DATA, 		SYSTEM_PARTITION_PHY_DATA_ADDR, 			0x1000 },
		{SYSTEM_PARTITION_SYSTEM_PARAMETER,	SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 	0x3000 },
};
const airkiss_config_t akconf = {
		(airkiss_memset_fn) &memset,
		(airkiss_memcpy_fn) &memcpy,
		(airkiss_memcmp_fn) &memcmp, 0,
};


void ICACHE_FLASH_ATTR user_init(void) {
	uart_init(74880, 74880);	// 初始化串口波特率
	os_delay_us(10000);			// 等待串口稳定

	os_printf("\n-------------------------------\n");
	os_printf("SDK version:%s\n", system_get_sdk_version());
	//连接WiFi
	connect(connect_success);

}

/*
 * wifi连接成功回调函数
 */
void ICACHE_FLASH_ATTR connect_success(void){
	os_printf("hello word!\n");
	start_sntp();
}

void ICACHE_FLASH_ATTR user_pre_init(void) {
	if (!system_partition_table_regist(at_partition_table,
			sizeof(at_partition_table) / sizeof(at_partition_table[0]),
			SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\n");
		while (1)
			;
	}
}
