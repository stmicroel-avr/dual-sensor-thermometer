#include "uart.h"

#include <stdint.h>
#include <stdbool.h>

#include "../board/board.h"
#include "../lib/uart/uart.h"
#include "../shared/temperature_string_state.h"

/**
 * Отправка тестовой строки
 *
 * @param elapsed_ms
 * @return bool
 */
bool uart_hello(uint32_t elapsed_ms) {
    static uint32_t start_ms = 0;
    if (!start_ms) {
        start_ms = elapsed_ms;
        return true;
    }

    if ((elapsed_ms - start_ms) < 5000) {
        return true;
    }

    uart_print("Hello world!");

    start_ms = elapsed_ms;

    return true;
}

/**
 * Дублирование температуры в реалтайме в UART
 *
 * @param elapsed_ms Тики таймера
 * @return bool
 */
bool uart_touch_temp(uint32_t elapsed_ms) {
    static uint32_t start_ms = 0;
    if (!start_ms) {
        start_ms = elapsed_ms;
        return true;
    }

    if ((elapsed_ms - start_ms) < MIRROR_TEMPERATURE_TIMEOUT) {
        return true;
    }

    start_ms = elapsed_ms;

    if (!state_ready_for_write()) {
        return true;
    }

    char s[30];
    if (serialize_state_to_string_buff(s)) {
        uart_print(s);
        reset_string_state();
    }

    return true;
}