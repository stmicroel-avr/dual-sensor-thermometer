#pragma once

#include <stdint.h>
#include <stdbool.h>

bool show_startup_screen(uint32_t elapsed_ms);
bool unactive_sleep_screen(uint32_t elapsed_ms);
bool active_screen(uint32_t elapsed_ms);