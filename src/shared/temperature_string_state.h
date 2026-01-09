#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool has_temp1;
    bool has_temp2;
    char temp1[8];
    char temp2[8];
} string_state_t;

extern string_state_t s_state;

/**
 * Данные кадра полноценны и готовы к записи в eeprom
 *
 * @return
 */
bool state_ready_for_write(void);

/**
 * Установить значение температуры датчика
 *
 * @param sensor_pin Номер пина датчика
 * @param temp_value Значение температуры в виде строкового буффера
 */
void set_string_temperature(uint8_t sensor_pin, const char *temp_value);

/**
 * Сериализовать кадр в буффер байтов
 *
 * @param buff Байтовый буффер
 * @return
 */
bool serialize_state_to_string_buff(char *buff);

/**
 * Сбросить состояние
 */
void reset_string_state(void);