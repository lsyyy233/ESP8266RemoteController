#include "c_types.h"
#include "led_green.h"
#include "eagle_soc.h"
#include "osapi.h"
#include "gpio.h"

BOOL flag_green = false;

void ICACHE_FLASH_ATTR led_green_init() {
	PIN_FUNC_SELECT(green_pin_name, green_pin_func);
	os_delay_us(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(green_pin_gpio_num), 0);
	os_delay_us(1000);
}
void ICACHE_FLASH_ATTR led_green_on() {
	if (flag_green == false) {
		flag_green = true;
		GPIO_OUTPUT_SET(GPIO_ID_PIN(green_pin_gpio_num), flag_green);
		os_delay_us(1000);
	}
}
void ICACHE_FLASH_ATTR led_green_off() {
	if (flag_green == true) {
		flag_green = false;
		GPIO_OUTPUT_SET(GPIO_ID_PIN(green_pin_gpio_num), flag_green);
		os_delay_us(1000);
	}
}
