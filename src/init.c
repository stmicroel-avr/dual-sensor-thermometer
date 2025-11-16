#include "bsp/board.h"
#include "utils/task_queue.h"
#include "lib/display/ssd1306xled.h"
#include "hal/hal_gpio.h"
#include "lib/temperature/ds18b20_nb.h"

void init_all(void) {
	// Display
	ssd1306_init();
	ssd1306_clear();
	// Task queue
	task_queue_init();
	// Led port
	hal_led_init();
	// Temp sensors
	ds18b20_set_resolution(DS_A, 11);
	ds18b20_set_resolution(DS_B, 11);
}