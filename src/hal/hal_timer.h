#pragma once

#include <stdint.h>

void hal_timer_init(void);
void hal_timer_start(void);
void hal_timer_stop(void);
uint32_t hal_ticks(void);