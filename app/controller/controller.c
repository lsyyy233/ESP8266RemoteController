#include "c_types.h"
#include "eagle_soc.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"

#include "controller.h"
#include "gpio_info.h"

void ICACHE_FLASH_ATTR init_controller_pin() {
	PIN_FUNC_SELECT(power_pin_name, power_pin_func);
	PIN_FUNC_SELECT(restart_pin_name, restart_pin_func);
	os_delay_us(1000);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(power_pin_gpio_num), 1);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(restart_pin_gpio_num), 1);
	os_delay_us(1000);
}

void ICACHE_FLASH_ATTR restart() {
	GPIO_OUTPUT_SET(GPIO_ID_PIN(restart_pin_gpio_num), 0);
	for (int i = 0; i <= 7; i++) {
		os_delay_us(100000);
		i += 1;
		system_soft_wdt_feed();
	}
	GPIO_OUTPUT_SET(GPIO_ID_PIN(restart_pin_gpio_num), 1);
}
void ICACHE_FLASH_ATTR power_off() {
	GPIO_OUTPUT_SET(GPIO_ID_PIN(power_pin_gpio_num), 0);
	for (int i = 0; i <= 30; i++) {
		os_delay_us(100000);
		i += 1;
		system_soft_wdt_feed();
	}
	GPIO_OUTPUT_SET(GPIO_ID_PIN(power_pin_gpio_num), 1);
}
void ICACHE_FLASH_ATTR power_on() {
	GPIO_OUTPUT_SET(GPIO_ID_PIN(power_pin_gpio_num), 0);
	for (int i = 0; i <= 7; i++) {
		os_delay_us(100000);
		i += 1;
		system_soft_wdt_feed();
	}
	GPIO_OUTPUT_SET(GPIO_ID_PIN(power_pin_gpio_num), 1);
}
