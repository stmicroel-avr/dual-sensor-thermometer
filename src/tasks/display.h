#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Отобразить стартовый экран
 *
 * @return bool
 */
void show_startup_screen(void);

/**
 * Деактивировать дисплей
 *
 * @param elapsed_ms Кол-во милисекунд с момент старта таймера
 * @return bool
 */
bool delayed_unactive_sleep_screen(uint32_t elapsed_ms);

/**
 * Активировать дисплей
 *
 * @param elapsed_ms Кол-во милисекунд с момент старта таймера
 * @return bool
 */
bool activate_screen(uint32_t elapsed_ms);