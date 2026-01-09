#include <stdio.h>
#include <stdbool.h>

#include "../lib/ds3232_rtc/ds3232_rtc.h"
#include "../lib/ssd1306_oled/ssd1306xled.h"
#include "../shared/temperature_frame_state.h"

#define X_POS 0
#define DISPLAY_PAGE 2

// Время инициализации
const rtc_time_t start_at = {
    .seconds = 0,
    .minutes = 40,
    .hours   = 13,
    .day     = 5,
    .date    = 7,
    .month   = 1,
    .year    = 26
};

// Текущее время
static rtc_time_t now = {
    .seconds = 0,
    .minutes = 0,
    .hours   = 0,
    .day     = 0,
    .date    = 0,
    .month   = 0,
    .year    = 0
};

/**
 * Инициализация RTC
 *
 * @return
 */
void init_rtc_time() {
    rtc_get_time(&now);
    if (!now.year) {
        rtc_set_time(&start_at);
    }
}

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
    char line[21];
    sprintf(
        line,
        "20%d-%02d-%02d %02d:%02d:%02d ",
        now.year,
        now.month,
        now.date,
        now.hours,
        now.minutes,
        now.seconds
    );
    ssd1306_puts6x8(X_POS, DISPLAY_PAGE, line);

    set_frame_time(now.year, now.month, now.date, now.hours, now.minutes);

    cycle_time_ms = elapsed_ms;

    return true;
}