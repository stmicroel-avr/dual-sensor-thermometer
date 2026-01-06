#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

#include "../lib/ssd1306_oled/ssd1306xled.h"
#include "../bsp/board.h"

/**
 * Отобразить стартовый экран
 *
 * @param elapsed_ms Кол-во милисекунд с момент старта таймера
 * @return bool
 */
bool show_startup_screen(uint32_t elapsed_ms) {
    (void)elapsed_ms;

    ssd1306_puts6x8(0, 0, "SmartTherm v1.2");
    ssd1306_puts6x8(0, 1, "Init...");
    ssd1306_puts6x8(0, 2, "Email: stap51@ya.com");
    ssd1306_puts6x8(0, 3, "Powered by STmicroel");

    _delay_ms(3000);

    ssd1306_clear();

    return false;
}

/**
 * Деактивировать дисплей
 *
 * @param elapsed_ms Кол-во милисекунд с момент старта таймера
 * @return bool
 */
bool delayed_unactive_sleep_screen(uint32_t elapsed_ms) {
    static uint32_t start_ms = 0;
    if (!start_ms) {
        start_ms = elapsed_ms;
    }

    if ((elapsed_ms-start_ms) >= DISPLAY_ACTIVE_MS_TIME) {
        ssd1306_puts6x8(0, 3, "Sleep mode on ");
        ssd1306_sleep();
        start_ms = 0;
        return false;
    }

    return true;
}

/**
 * Активировать дисплей
 *
 * @param elapsed_ms Кол-во милисекунд с момент старта таймера
 * @return bool
 */
bool activate_screen(uint32_t elapsed_ms) {
    (void)elapsed_ms;

    ssd1306_wakeup();

    return true;
}