#include "eagle_soc.h"
#include "c_types.h"

#define WIFI_STATUS_PIN_NAME 	PERIPHS_IO_MUX_MTDO_U
#define WIFI_STATUS_PIN_FUNC 	FUNC_GPIO15
#define WIFI_STATUS_GPIO_NUM 	15

void init_wifi_led();
void set_wifi_led_ok();
void set_wifi_led_error();
