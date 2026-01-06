#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pwr.h"

#include <time.h>

#include "../lib/ssd1306_oled/ssd1306xled.h"

/**
 * Отразить время
 *
 * @param elapsed_ms Тики таймера
 */
bool display_pwr(uint32_t elapsed_ms) {
    (void)elapsed_ms;

    static uint32_t now = 0;

    if ((elapsed_ms - now) < 500) {
        return true;
    }

    char line[21];
    uint16_t mv = readVcc();
    snprintf(line, sizeof(line), "VCC %u.%02uV", mv / 1000, (mv % 1000) / 10);

    ssd1306_puts6x8(0, 2, line);

    now = elapsed_ms;

    return true;
}