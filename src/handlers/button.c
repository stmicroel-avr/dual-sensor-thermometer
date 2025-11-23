#include <stdint.h>
#include <stdbool.h>

#include "button.h"
#include "display.h"
#include "ssd1306xled.h"
#include "../hal/hal_btn.h"
#include "../utils/task_queue.h"

typedef enum {
    BTN_IDLE,        // кнопка отпущена
    BTN_WAIT_STABLE, // заметили нажатие, ждём окончания дребезга
    BTN_PRESSED      // подтверждённое нажатие, ждём отпускания
} btn_state_t;

/**
 *  Обработчик нажатия кнопки
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
bool check_btn_press(uint32_t elapsed_ms) {
    static btn_state_t state = BTN_IDLE;
    static uint32_t t0 = 0;

    bool pressed = hal_is_button_pressed();

    switch (state) {
        case BTN_IDLE:
            // увидели переход в "нажата" → стартуем таймер дребезга
            if (pressed) {
                t0 = elapsed_ms;
                state = BTN_WAIT_STABLE;
            }
            break;
        case BTN_WAIT_STABLE:
            // если за время дребезга кнопка отпустилась — ложное срабатывание, назад в IDLE
            if (!pressed) {
                state = BTN_IDLE;
            } else if ((elapsed_ms - t0) >= DEBOUNCE_TIME) {
                // кнопка стабильно нажата достаточно долго → считаем настоящим нажатием
                ssd1306_wakeup();
                ssd1306_puts6x8(0, 3, "             ");
                task_queue_add_job(delayed_unactive_sleep_screen);

                state = BTN_PRESSED;
            }
            break;
        case BTN_PRESSED:
            // ждём отпускания, чтобы разрешить следующее нажатие
            if (!pressed) {
                state = BTN_IDLE;
            }
            break;
    }
    return true;
}

