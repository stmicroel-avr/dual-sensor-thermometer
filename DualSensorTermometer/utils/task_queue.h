#pragma once

#include <stdint.h>
#include <stdbool.h>

void task_queue_init(void);
bool task_queue_add_job(bool (*handler)(uint32_t));
void task_queue_handle(void);
uint8_t task_queue_get_count(void);