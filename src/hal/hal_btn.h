#pragma once

#include <stdbool.h>

/**
 * Инициализация порта кнопки
 */
void hal_btn_init(void);

/**
 * Была ли нажата кнопка
 */
bool hal_is_button_pressed(void);