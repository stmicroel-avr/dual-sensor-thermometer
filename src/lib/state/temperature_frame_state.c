#include "temperature_frame_state.h"

#include <stdbool.h>
#include <stdint.h>

#include "board_atmega328pb.h"

/**
 * Хранение данных кадра лога для eeprom
 */
volatile frame_state_t t_state_frame = {
    .has_time = false,
    .has_temp1 = false,
    .has_temp2 = false,
    .minutes_since = 0,
    .last_touch_time = 0,
    .temp1_x100 = 0,
    .temp2_x100 = 0
};

static const uint16_t year_days[12] = {
    0, // Янв 1
    31, // Фев 1
    59, // Мар 1
    90, // Апр 1
    120, // Май 1
    151, // Июнь 1
    181, // Июль 1
    212, // Авг 1
    243, // Сент 1
    273, // Окт 1
    304, // Ноя 1
    334, // Дек 1
};

/**
 * Проверка на високосный год
 *
 * @param year
 * @return
 */
static uint8_t is_leap_year(uint16_t year) {
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

/**
 * Получить разность между текущей и опорной датой
 *
 * @param year Год(с 2000)
 * @param month Месяц
 * @param day День
 * @param hour Часы
 * @param minutes Минуты
 */
uint32_t get_minutes_since_threshold(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minutes) {
    uint16_t full_year = 2000 + year;
    if (full_year < TEMPERATURE_FRAME_START_YEAR || month < 1 || month > 12 || day < 1 || day > 31 || hour > 23 || minutes > 59) {
        return UINT32_MAX;
    }

    uint32_t minutes_since = 0;
    for (uint16_t y = TEMPERATURE_FRAME_START_YEAR; y < full_year; y++) {
        minutes_since += (is_leap_year(y) ? 366UL : 365UL) * 1440UL;
    }

    minutes_since += year_days[month - 1] * 1440UL;

    if (month > 2 && is_leap_year(full_year)) {
        minutes_since += 1440UL;
    }

    minutes_since += (day - 1) * 1440UL;
    minutes_since += hour * 60UL;
    minutes_since += minutes;

    return minutes_since;
}

/**
 * Установить время(в заданном формате)
 *
 * @param year Год(с 2000)
 * @param month Месяц
 * @param date День
 * @param hour Часы
 * @param minutes Минуты
 */
void set_frame_time(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minutes) {
    uint32_t minutes_since = get_minutes_since_threshold(year, month, date, hour, minutes);
    if (minutes_since == UINT32_MAX) {
        return;
    }
    t_state_frame.has_time = true;
    t_state_frame.minutes_since = minutes_since;
}

/**
 * Установить значение температуры датчика
 *
 * @param sensor_pin Номер пина датчика
 * @param temp_value Значение температуры в виде двухбайтного числа
 */
void set_frame_temperature(uint8_t sensor_pin, uint16_t temp_value) {
    if (sensor_pin == DS_A) {
        t_state_frame.has_temp1 = true;
        t_state_frame.temp1_x100 = temp_value;
    } else if (sensor_pin == DS_B) {
        t_state_frame.has_temp2 = true;
        t_state_frame.temp2_x100 = temp_value;
    }
}

/**
 * Данные кадра полноценны и готовы к записи в eeprom
 *
 * @return
 */
bool frame_ready_for_write(void) {
    return t_state_frame.has_time
    && t_state_frame.has_temp1
    && t_state_frame.has_temp2
    && (!t_state_frame.last_touch_time || t_state_frame.last_touch_time != t_state_frame.minutes_since);
}