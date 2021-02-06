#include "led_white.h"
#include "c_types.h"
#include "eagle_soc.h"
#include "osapi.h"
#include "gpio.h"

BOOL flag_white = false;

void ICACHE_FLASH_ATTR led_white_init() {
	PIN_FUNC_SELECT(white_pin_name, white_pin_func);
	os_delay_us(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(white_pin_gpio_num), 0);
	os_delay_us(1000);
}
void ICACHE_FLASH_ATTR led_white_on() {
	if (flag_white == false) {
		flag_white = true;
		GPIO_OUTPUT_SET(GPIO_ID_PIN(white_pin_gpio_num), flag_white);
		os_delay_us(1000);
	}
}
void ICACHE_FLASH_ATTR led_white_off() {
	if (flag_white == true) {
		flag_white = false;
		GPIO_OUTPUT_SET(GPIO_ID_PIN(white_pin_gpio_num), flag_white);
		os_delay_us(1000);
	}
}

void ICACHE_FLASH_ATTR led_white_quick_flash(){
	led_white_on();
	os_delay_us(100000);
	led_white_off();
}
