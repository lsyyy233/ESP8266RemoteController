#include "smartconfig.h"
#include "STA_INFO.h"


typedef void connect_success_callback(void);

void  OS_Timer_IP_cb();
void smartconfig_done(sc_status status, void *pdata);
void connect(connect_success_callback *func);
void start_smartconfig();
void smartconfig_done(sc_status status, void *pdata);
