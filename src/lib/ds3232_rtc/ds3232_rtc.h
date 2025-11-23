#pragma once
#include <stdint.h>

/**
 * Структура представления времени DS3232/DS3231.
 * Все значения в обычном бинарном формате (не BCD).
 */
typedef struct {
    uint8_t seconds;   // 0–59
    uint8_t minutes;   // 0–59
    uint8_t hours;     // 0–23, 24h формат

    uint8_t day;       // 1–7 (номер дня недели)
    uint8_t date;      // 1–31 (число месяца)
    uint8_t month;     // 1–12
    uint8_t year;      // 0–99 (20xx)
} rtc_time_t;

/**
 * Записывает время и дату в DS3232.
 * Все поля структуры должны быть заданы в бинарном формате.
 */
void rtc_set_time(const rtc_time_t *t);

/**
 * Считывает текущее время и дату из DS3232.
 * Заполняет структуру значениями в бинарном формате.
 */
void rtc_get_time(rtc_time_t *t);
