/*
 * DualSensorThermometer.c
 *
 * Created: 03.11.2025 0:27:24
 * Author : stmicroel-avr
 */ 

#include "utils/task_queue.h"
#include "hal/hal_gpio.h"
#include "handlers/led.h"
#include "lib/display/ssd1306xled.h"
#include "lib/display/font6x8.h"

// Entrypoint
int main(void) {
	// 1. Initialize display
	ssd1306_init();
	// 2. Clear screen
	ssd1306_clear();
	// 3. Show start screen
	ssd1306_puts6x8(0, 0, "***** STMICROEL *****");
	ssd1306_puts6x8(0, 2, "Initialization...");
	// 4. Initialize all task queue timers
	task_queue_init();
	// 5. Initialize led port
	hal_led_init();
	// 6. Add led_power_on_job
	task_queue_add_job(led_power_on);
	
	while(1) {
		// Handle queue
		task_queue_handle();
	}
}


