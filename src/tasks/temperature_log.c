#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "../board/board.h"
#include "temperature_log.h"
#include "lib/24c64/24cxx_eeprom.h"
#include "lib/ssd1306_oled/ssd1306xled.h"
#include "../shared/temperature_frame_state.h"

/**
 * Проверка наличия данных в буффере на запись
 *
 * @param elapsed_ms Тики таймера
 * @return bool
 */
bool check_log_frame_set(uint32_t elapsed_ms) {
    static uint32_t start_ms = 0;
    if (!start_ms) {
        start_ms = elapsed_ms;
        return true;
    }

    if ((elapsed_ms - start_ms) <= EEPROM_WRITE_CHECK_FRAME_TIMEOUT) {
        return true;
    }

    start_ms = elapsed_ms;

    if (!frame_ready_for_write()) {
        return true;
    }

    uint8_t buff[8];
    serialize_state_to_data_buff(buff);
    uint16_t next_mem_addr = write_data_to_ring_buffer(buff, 8);

    reset_state_frame();

    // отобразим сл адрес на экране
    char display_buff[11];
    sprintf(display_buff, "EADDR: %u", next_mem_addr);
    ssd1306_puts6x8(10*6, 3, display_buff);

    return true;
}