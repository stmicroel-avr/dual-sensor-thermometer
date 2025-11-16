#include <stdbool.h>
#include <stdint.h>

#include "../lib/display/ssd1306xled.h"

bool show_startup_screen(uint32_t elapsed_ms) {
    ssd1306_puts6x8(0, 0, "***** STMICROEL *****");
    ssd1306_puts6x8(0, 2, "Initialization...");
    return false;
}

bool unactive_sleep_screen(uint32_t elapsed_ms) {
    if (elapsed_ms > 10000) {
        ssd1306_sleep();
        return false;
    }
    return true;
}

bool active_screen(uint32_t elapsed_ms) {
    if (elapsed_ms > 20000) {
        ssd1306_wakeup();
        return false;
    }
    return true;
}