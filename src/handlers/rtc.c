#include <stdio.h>
#include <stdbool.h>

#include "../lib/ds3232_rtc/ds3232_rtc.h"
#include "../lib/ssd1306_oled/ssd1306xled.h"

#define X_POS 0
#define DISPLAY_PAGE 3

static rtc_time_t now = {
    .seconds = 0,
    .minutes = 0,
    .hours   = 0,
    .day     = 7,
    .date    = 23,
    .month   = 11,
    .year    = 25
};

/**
 * Обновить время на дисплее
 *
 * @param elapsed_ms Тики таймера
 * @return uint32_t
 */
bool display_rtc_time(uint32_t elapsed_ms) {
    static uint32_t cycle_time_ms = 0;

    if (!cycle_time_ms) {
        cycle_time_ms = elapsed_ms;
        return true;
    }

    if ((elapsed_ms - cycle_time_ms) <= 1000) {
        return true;
    }

    rtc_get_time(&now);
    char line[15];
    sprintf(line, "Time: %d:%d:%d", now.hours, now.minutes, now.seconds);
    ssd1306_puts6x8(X_POS, DISPLAY_PAGE, line);
    cycle_time_ms = elapsed_ms;

    return true;
}