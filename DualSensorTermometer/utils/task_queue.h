/*
 * timer_dispatcher.h
 *
 * Created: 05.11.2025 20:07:52
 *  Author: Stepan
 */ 


#pragma once

#include <stdint.h>
#include <stdbool.h>

// Initialize all timers
void task_queue_init(void);

// Add job in queue
bool task_queue_add_job(bool (*handler)(uint32_t));

// Handle all tasks in queue
void task_queue_handle(void);

// Get active tasks in queue
uint8_t task_queue_get_count(void);