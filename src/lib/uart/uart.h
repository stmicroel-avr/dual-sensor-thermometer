#pragma once

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