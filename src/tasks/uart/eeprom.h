#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Прочитать и отправить все объекты из EEPROM
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
bool send_eeprom_frames(uint32_t elapsed_ms);