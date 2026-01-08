#include <stdint.h>

#include "eeprom.h"
#include "../bsp/board.h"

/**
 * Проверка наличия данных в буффере на запись
 *
 * @param elapsed_ms Тики таймера
 * @return bool
 */
bool check_write_frame_set(uint32_t elapsed_ms) {
    static uint32_t start_ms = 0;
    if (!start_ms) {
        start_ms = elapsed_ms;
        return true;
    }

    if ((elapsed_ms - start_ms) <= EEPROM_WRITE_CHECK_FRAME_TIMEOUT) {
        return true;
    }

    start_ms = elapsed_ms;

    return true;
}