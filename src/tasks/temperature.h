#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../board/board.h"

// Номер строки для отображения температуры с датчика А
#define PAGE_A 0u
// Номер строки для отображения температуры с датчика В
#define PAGE_B 1u

/**
 * Обработка этапа считывания информации с датчика
 *
 * @param pin_bit Пин порта датчика
 * @param page Номер строки
 * @param label Префикс строки
 * @return
 */
void sensor_fsm_step(uint8_t pin_bit, uint8_t page, const char *label);

/**
 * Обработчик вывода температуры датчика А
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
__attribute__((unused))
static bool print_temp_a(uint32_t elapsed_ms) {
    (void)elapsed_ms;
    sensor_fsm_step(DS_A, PAGE_A, "On board:");
    return true;
}

/**
 * Обработчик вывода температуры датчика B
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
__attribute__((unused))
static bool print_temp_b(uint32_t elapsed_ms) {
    (void)elapsed_ms;
    sensor_fsm_step(DS_B, PAGE_B, "External:");
    return true;
}