#pragma once

#include <stdint.h>

/**
 * Инициализация аппаратного I2C
 */
void twi_init(void);

/**
 * Старт передачи данных
 *
 * @param addr Адрес устройства
 */
void twi_start(uint8_t addr);

/**
 * Конец передачи данных
 */
void twi_stop(void);

/**
 * Отправить данные
 *
 * @param data Данные
 */
void twi_write(uint8_t data);

/**
 * Получить следующий байт
 *
 * @return uint8_t
 */
uint8_t twi_read_ack(void);

/**
 * Получить последний байт и завершить передачу
 *
 * @return uint8_t
 */
uint8_t twi_read_nack(void);