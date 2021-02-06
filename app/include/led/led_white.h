#include "eagle_soc.h"
#include "c_types.h"

#define white_pin_name 		PERIPHS_IO_MUX_GPIO5_U
#define white_pin_func 		FUNC_GPIO5
#define white_pin_gpio_num 	5

void led_white_init();
void led_white_on();
void led_white_off();
void led_white_quick_flash();
