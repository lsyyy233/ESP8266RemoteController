#include "sntp_init.h"
#include "sntp.h"
#include "ets_sys.h"
#include "osapi.h"

static ETSTimer sntp_timer;

void ICACHE_FLASH_ATTR start_sntp(){
	sntp_setservername(0,SNTP_SERVER_1);
	sntp_setservername(1,SNTP_SERVER_2);
	sntp_setservername(3,SNTP_SERVER_3);
	sntp_init();
	os_delay_us(1000000);

	// ����SNTP��ʱ��[sntp_timer]
	//-----------------------------------------------------------
	os_timer_disarm(&sntp_timer);
	os_timer_setfn(&sntp_timer, (os_timer_func_t *)sntpfn, NULL);
	os_timer_arm(&sntp_timer, 1000, 1);		// 1s��ʱ
}

/*
 * SNTP��ʱ��������ȡ��ǰ����ʱ��
 */
void ICACHE_FLASH_ATTR sntpfn()
{
    u32_t ts = 0;

    ts = sntp_get_current_timestamp();		// ��ȡ��ǰ��ƫ��ʱ��
    os_printf("current time : %s\n", sntp_get_real_time(ts));	// ��ȡ��ʵʱ��
    if (ts == 0)		// ����ʱ���ȡʧ��
    {
        os_printf("did not get a valid time from sntp server\n");
    }
    else //(ts != 0)	// ����ʱ���ȡ�ɹ�
    {
            os_timer_disarm(&sntp_timer);	// �ر�SNTP��ʱ��
//            MQTT_Connect(&mqttClient);		// ��ʼMQTT����
    }
}
