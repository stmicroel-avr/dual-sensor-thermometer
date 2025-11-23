#pragma once

#include <stdint.h>

/**
 * Инициализация таймера
 */
void hal_timer_init(void);

/**
 * Запуск таймера
 */
void hal_timer_start(void);

/**
 * Остановка таймера
 */
void hal_timer_stop(void);

/**
 * Получить значение таймера
 *
 * @return uint32_t
 */
uint32_t hal_ticks(void);