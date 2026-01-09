#pragma once

#include <stdint.h>

/**
 * Записать буффер байт в кольцевой буффер
 *
 * @param buff Указатель на буффер
 * @param len Длина буффера
 */
uint16_t write_data_to_ring_buffer(uint8_t *buff, uint8_t len);