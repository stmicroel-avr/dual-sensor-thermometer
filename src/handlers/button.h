#pragma once

#include <stdint.h>
#include <stdbool.h>

// Время задержки антидребезга
#define DEBOUNCE_TIME 20

/**
 * Обработчик нажатия кнопки с антидребезгом
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
bool check_btn_press(uint32_t elapsed_ms);