#pragma once

#include <stdbool.h>

/**
 * Инициализация UART
 */
void uart_init(void);

/**
 * Отравить строку по UART
 *
 * @param text Строка
 */
void uart_print(const char *text);

/**
 * Прочитать строку UART
 *
 * @param out Буффер для чтения
 * @return
 */
bool uart_poll_read(char *out);