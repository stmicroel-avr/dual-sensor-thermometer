#include "../board/board.h"
#include "uart.h"

#include <avr/io.h>
#include <stdbool.h>

/**
 * Инициализация UART
 */
void uart_init(void) {
    // 9600 baud @ 8MHz → UBRR = 51
    UBRR0H = 0;
    UBRR0L = 51;

    // Включаем передатчик и приемник
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

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

/**
 * Чтения буффера UART
 *
 * @param out Полученные данные
 * @return
 */
bool uart_poll_read(char *out) {
    static uint8_t idx = 0;

    while (UCSR0A & (1 << RXC0)) {
        char c = UDR0;
        if (c == '\r') {
            continue;
        }

        if (c == '\n') {
            out[idx] = '\0';
            idx = 0;
            return true;
        }

        if (idx < UART_CMD_BUFFER - 1) {
            out[idx] = c;
            idx++;
        }
    }

    return false;
}
