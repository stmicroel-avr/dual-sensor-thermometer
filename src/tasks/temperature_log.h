#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Проверка наличия данных в буффере на запись
 *
 * @param elapsed_ms Тики таймера
 * @return bool
 */
bool check_log_frame_set(uint32_t elapsed_ms);