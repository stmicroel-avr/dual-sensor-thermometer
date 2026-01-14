#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Тестовый метод UART
 *
 * @param elapsed_ms Тики таймера
 * @return bool
 */
bool uart_hello(uint32_t elapsed_ms);

/**
 * Дублирование температуры в реалтайме в UART
 *
 * @param elapsed_ms Тики таймера
 * @return bool
 */
bool uart_touch_temp(uint32_t elapsed_ms);

/**
 * Прочитать команду (если пришла)
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
bool uart_read_cmd(uint32_t elapsed_ms);