#include "eagle_soc.h"
#include "c_types.h"

#define green_pin_name 		PERIPHS_IO_MUX_GPIO4_U
#define green_pin_func 		FUNC_GPIO4
#define green_pin_gpio_num 	4

void led_green_init();
void led_green_on();
void led_green_off();
