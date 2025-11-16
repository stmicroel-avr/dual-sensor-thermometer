#pragma once

#include <stdint.h>
#include <stdbool.h>

// Resolution of sensor
void ds18b20_set_resolution(uint8_t bit, uint8_t res_bits);

// API
bool ds18b20_start(uint8_t bit);
bool ds18b20_ready(uint8_t bit);                
bool ds18b20_read_temp(uint8_t bit, int16_t *t);