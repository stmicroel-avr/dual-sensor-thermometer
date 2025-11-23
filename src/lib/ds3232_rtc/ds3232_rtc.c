#include <stdint.h>

#include "../twi/twi.h"
#include "ds3232_rtc.h"

/**
 * 7-битный адрес DS3232 (0x68).
 * При передаче в twi_start добавляем бит R/W вручную.
 */
#define DS3232_ADDR        0x68
#define DS3232_ADDR_WRITE  ((DS3232_ADDR << 1) | 0)
#define DS3232_ADDR_READ   ((DS3232_ADDR << 1) | 1)

/**
 * Конвертация бинарного числа в BCD (Binary Coded Decimal).
 */
static uint8_t bin2bcd(uint8_t val) {
    return (uint8_t)(((val / 10) << 4) | (val % 10));
}

/**
 * Конвертация BCD в обычный бинарный формат.
 */
static uint8_t bcd2bin(uint8_t val) {
    return (uint8_t)(((val >> 4) * 10) + (val & 0x0F));
}

/**
 * Записывает время и дату в DS3232.
 * Записываются первые 7 регистров: секунд, минут, часов,
 * дня недели, числа, месяца и года.
 */
void rtc_set_time(const rtc_time_t *t) {
    if (!t) {
        return;
    }

    uint8_t sec   = bin2bcd(t->seconds) & 0x7F; // бит7 CH должен быть 0
    uint8_t min   = bin2bcd(t->minutes) & 0x7F;
    uint8_t hour  = bin2bcd(t->hours)   & 0x3F; // 24-часовой формат

    uint8_t day   = bin2bcd(t->day   ? t->day   : 1);
    uint8_t date  = bin2bcd(t->date  ? t->date  : 1);
    uint8_t month = bin2bcd(t->month ? t->month : 1) & 0x1F;
    uint8_t year  = bin2bcd(t->year);

    twi_start(DS3232_ADDR_WRITE);
    twi_write(0x00);        // начальный регистр (секунды)

    twi_write(sec);
    twi_write(min);
    twi_write(hour);
    twi_write(day);
    twi_write(date);
    twi_write(month);
    twi_write(year);

    twi_stop();
}

/**
 * Считывает 7 регистров времени/даты из DS3232
 * начиная с регистра 0x00.
 */
void rtc_get_time(rtc_time_t *t) {
    if (!t) {
        return;
    }

    /* Устанавливаем указатель регистра на 0x00 */
    twi_start(DS3232_ADDR_WRITE);
    twi_write(0x00);

    /* Чтение последовательности из 7 байт */
    twi_start(DS3232_ADDR_READ);

    uint8_t sec_bcd   = twi_read_ack();
    uint8_t min_bcd   = twi_read_ack();
    uint8_t hour_bcd  = twi_read_ack();
    uint8_t day_bcd   = twi_read_ack();
    uint8_t date_bcd  = twi_read_ack();
    uint8_t month_bcd = twi_read_ack();
    uint8_t year_bcd  = twi_read_nack(); // последний байт — NACK

    twi_stop();

    /* Преобразование в обычные числа */
    t->seconds = bcd2bin(sec_bcd & 0x7F);
    t->minutes = bcd2bin(min_bcd & 0x7F);
    t->hours   = bcd2bin(hour_bcd & 0x3F);

    t->day     = bcd2bin(day_bcd & 0x07);
    t->date    = bcd2bin(date_bcd & 0x3F);
    t->month   = bcd2bin(month_bcd & 0x1F);
    t->year    = bcd2bin(year_bcd);
}
