#include "uart.h"

#include <avr/io.h>

/**
 * Инициализация UART
 */
void uart_init(void) {
    // 9600 baud @ 8MHz → UBRR = 51
    UBRR0H = 0;
    UBRR0L = 51;

    // Включаем передатчик
    UCSR0B = (1 << TXEN0);

    // 8 бит, 1 стоп, без чётности
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/**
 * Отправить байт
 *
 * @param c  байт
 */
void uart_tx(char c) {
    while (!(UCSR0A & (1 << UDRE0))); // ждём, пока буфер освободится
    UDR0 = c;
}

/**
 * Отправить строку по UART
 *
 * @param text Строка
 */
void uart_print(const char *text) {
    while (*text) {
        uart_tx(*text++);
    }
}
