#include "temperature_string_state.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "board_atmega328pb.h"

/**
 * Хранение данных кадра лога для eeprom
 */
string_state_t s_state = {
    .has_temp1 = false,
    .has_temp2 = false,
    .temp1 = "",
    .temp2 = ""
};

/**
 * Установить значение температуры датчика
 *
 * @param sensor_pin Номер пина датчика
 * @param temp_value Значение температуры в виде строкового буффера
 */
void set_string_temperature(uint8_t sensor_pin, const char *temp_value) {
    if (sensor_pin == DS_A) {
        s_state.has_temp1 = true;
        strncpy(s_state.temp1, temp_value, sizeof(s_state.temp1) - 1);
        s_state.temp1[sizeof(s_state.temp1) - 1] = '\0';
    } else if (sensor_pin == DS_B) {
        s_state.has_temp2 = true;
        strncpy(s_state.temp2, temp_value, sizeof(s_state.temp2) - 1);
        s_state.temp2[sizeof(s_state.temp2) - 1] = '\0';
    }
}

/**
 * Данные кадра полноценны и готовы к записи в eeprom
 *
 * @return
 */
bool state_ready_for_write(void) {
    return s_state.has_temp1 && s_state.has_temp2;
}

/**
 * Сериализовать кадр в буффер байт
 *
 * @param buff Символьный буффер
 * @return
 */
bool serialize_state_to_string_buff(char *buff) {
    if (!state_ready_for_write()) {
        return false;
    }

    sprintf(buff, "Temp1:%s;Temp2:%s;", s_state.temp1, s_state.temp2);

    return true;
}

/**
 * Сбросить состояние
 */
void reset_string_state(void) {
    s_state.has_temp1 = false;
    s_state.has_temp2 = false;
    s_state.temp1[0] = '\0';
    s_state.temp2[0] = '\0';
}