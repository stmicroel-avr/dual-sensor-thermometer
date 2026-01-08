#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool has_time;
    bool has_temp1;
    bool has_temp2;
    uint32_t minutes_since;
    uint32_t last_touch_time;
    int16_t temp1_x100;
    int16_t temp2_x100;
} frame_state_t;

extern volatile frame_state_t t_state_frame;

/**
 * Данные кадра полноценны и готовы к записи в eeprom
 *
 * @return
 */
bool frame_ready_for_write(void);

/**
 * Установить значение температуры датчика
 *
 * @param sensor_pin Номер пина датчика
 * @param temp_value Значение температуры в виде двухбайтного числа
 */
void set_frame_temperature(uint8_t sensor_pin, int16_t temp_value);

/**
 * Установить время(в заданном формате)
 *
 * @param year Год(с 2000)
 * @param month Месяц
 * @param date День
 * @param hour Часы
 * @param minutes Минуты
 */
void set_frame_time(uint8_t year, uint8_t month, uint8_t date, uint8_t hour, uint8_t minutes);

/**
 * Сбросить фрейм
 */
void reset_state_frame(void);

/**
 * Сериализовать кадр в буффер байтов
 *
 * @param buff Байтовый буффер
 * @return
 */
bool serialize_state_to_data_buff(uint8_t *buff);