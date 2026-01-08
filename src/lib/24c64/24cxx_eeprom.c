#include <stdint.h>

#include "../twi/twi.h"
#include "../../board/board.h"

/**
 * Записать буффер байт в кольцевой буффер
 *
 * @param buff Указатель на буффер
 * @param len Длина буффера
 */
void write_data_to_ring_buffer(uint8_t *buff, uint8_t len) {
    buff[len - 1] = FRAME_VALID_MARKER;

    static uint16_t mem_addr = 0;

    if (mem_addr + len > EXT_EEPROM_SIZE) {
        mem_addr = 0;
    }

    twi_start(EXT_EEPROM_ADDR << 1);

    twi_write((uint8_t)(mem_addr >> 8));
    twi_write((uint8_t)(mem_addr & 0xFF));

    for (uint8_t i=0; i<len; i++) {
        twi_write(buff[i]);
    }

    twi_stop();

    mem_addr += len;
}