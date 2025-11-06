/*
 * led.h
 *
 * Created: 05.11.2025 21:35:07
 *  Author: Stepan
 */ 

#pragma once

#include <stdint.h>
#include <stdbool.h>

// Handler of power on (blink twice)
bool led_power_on(uint32_t elapsed_ms);