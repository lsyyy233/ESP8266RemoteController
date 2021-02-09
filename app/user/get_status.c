#include "c_types.h"
#include "eagle_soc.h"
#include "osapi.h"
#include "gpio.h"
#include "gpio_info.h"

#include "get_status.h"

void ICACHE_FLASH_ATTR get_status_pin_init(){
	PIN_FUNC_SELECT(get_status_pin_name, get_status_pin_func);
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(get_status_pin_gpio_num));
	PIN_PULLUP_EN(get_status_pin_name);
}

BOOL ICACHE_FLASH_ATTR get_status(){
	return GPIO_INPUT_GET(GPIO_ID_PIN(get_status_pin_gpio_num));
}
