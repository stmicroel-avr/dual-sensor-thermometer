#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "../bsp/board.h"

// Была ли нажата кнопка
static volatile bool button_pressed = false;

/**
 * Установка пина как вывода
 */
ISR(INT0_vect) {
    button_pressed = true;
}

/**
 * Инициализировать пин кнопки & Настроить прерывание
 */
void hal_btn_init() {
    // Конфигурим порт как вход
    BTN_DDR &= (1 << BTN_PIN);
    BTN_PORT |= (1 << BTN_PIN);

    // Разрешаем прерывание
    EICRA |=  (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    EIMSK |= (1 << INT0);
    sei();
}

/**
 * Была ли нажата кнопка
 */
bool hal_is_button_pressed(void) {
    return button_pressed;
}