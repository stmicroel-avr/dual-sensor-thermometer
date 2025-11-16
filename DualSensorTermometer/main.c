/*
 * DualSensorThermometer.c
 *
 * Created: 03.11.2025 0:27:24
 * Author : stmicroel-avr
 */ 

#include "bsp/board.h"
#include "utils/task_queue.h"
#include "handlers/display.h"
#include "handlers/led.h"
#include "handlers/temperature.h"
#include "init.h"

// Main function
int main(void) {
	// Initialize
	init_all();
	
	// Add jobs 
	task_queue_add_job(led_power_on);
	task_queue_add_job(show_startup_screen);
	task_queue_add_job(unactive_sleep_screen);
	task_queue_add_job(active_screen);
	task_queue_add_job(print_temp_a);
	task_queue_add_job(print_temp_b);
	
	// Infinite loop
	while (1) {
		task_queue_handle();
	}
}


