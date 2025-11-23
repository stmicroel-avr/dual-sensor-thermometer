#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 *  Мигнуть светодиодом на старте
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
bool led_power_on(uint32_t elapsed_ms);