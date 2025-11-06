/*
 * DualSensorThermometer.c
 *
 * Created: 03.11.2025 0:27:24
 * Author : stmicroel-avr
 */ 

#include "utils/task_queue.h"
#include "hal/hal_gpio.h"
#include "tasks/led.h"

// Entrypoint
int main(void) {
	// Initialize all task queue timers
	task_queue_init();
	
	// Initialize led port
	hal_led_init();
	
	// Add led_power_on_job
	task_queue_add_job(led_power_on);
	
	while(1) {
		// Handle queue
		task_queue_handle();
	}
}


