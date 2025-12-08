#pragma once

#include <stdint.h>

/**
 * Отразить время
 *
 * @param elapsed_ms Тики таймера
 */
bool display_rtc_time(uint32_t elapsed_ms);

/**
 * Инициализация RTC
 *
 * @return
 */
bool init_rtc_time(uint32_t elapsed_ms);