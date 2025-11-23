#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Задать разрешение датчика (макс 12 бит => 750ms)
 *
 * @param bit Пин датчика
 * @param res_bits Кол-во бит
 */
void ds18b20_set_resolution(uint8_t bit, uint8_t res_bits);

/**
 * Старт запуска конверсии температуры
 *
 * @param bit Пин датчика
 * @return
 */
bool ds18b20_start(uint8_t bit);

/**
 * Проверка готовности регистров буффера температуры
 *
 * @param bit Пин датчика
 * @return
 */
bool ds18b20_ready(uint8_t bit);

/**
 * Прочитать температуру из буффера(ds18b20_ready вернул true)
 *
 * @param bit Пин датчика
 * @param t Указатель на переменную записи
 * @return
 */
bool ds18b20_read_temp(uint8_t bit, int16_t *t);