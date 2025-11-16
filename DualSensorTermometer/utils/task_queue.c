#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../hal/hal_timer.h"

// Task size
#define QUEUE_SIZE 8

// Task struct
typedef struct {
	bool (*handler)(uint32_t);
	uint32_t start_ms;
	bool active;
} Task;

// Queue stack
static Task queue[QUEUE_SIZE];

// Initialize all timers
void task_queue_init(void) {
	// Fill queue
	for (uint8_t i = 0; i < QUEUE_SIZE; i++) {
		queue[i].active = false;
		queue[i].start_ms = 0;
		queue[i].handler = NULL;
	}
	
	// Initialize Timer0
	hal_timer_init();
	hal_timer_start();
}

// Add job in queue
bool task_queue_add_job(bool (*handler)(uint32_t)) {
	if (handler == NULL) {
		return false;
	}
	
	for (uint8_t i = 0; i < QUEUE_SIZE; i++) {
		if (!queue[i].active) {
			queue[i].start_ms = hal_ticks();
			queue[i].handler = handler;
			queue[i].active = true;
			return true;
		}
	}
	return false;
}

// Rotate queue and run handlers
void task_queue_handle(void) {
	uint32_t now = hal_ticks();
	
	for (uint8_t i = 0; i < QUEUE_SIZE; i++) {
		if (!queue[i].active) {
			continue;
		}
		
		uint32_t elapsed_ms = now - queue[i].start_ms;
		bool retry = queue[i].handler(elapsed_ms);
		
		// Unload task when handler return false
		if (!retry) {
			queue[i].active = false;
			queue[i].start_ms = 0;
			queue[i].handler = NULL;
		}
	}
}

// Get active task count
uint8_t task_queue_get_count(void) {
	uint8_t count = 0;
	for (uint8_t i = 0; i < QUEUE_SIZE; i++) {
		if (queue[i].active) {
			count++;
		}
	}
	return count;
}