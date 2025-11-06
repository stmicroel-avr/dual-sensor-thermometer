/*
 * hal_gpio.c
 *
 * Created: 03.11.2025 20:05:54
 *  Author: stmicroel-avr
 */ 

#include "../bsp/board.h"
#include "hal_gpio.h"

// Initialize led pin as output
void hal_led_init(void) {
	LED_DDR |= (1 << LED_PIN);
}

// Invert pin value
void hal_led_toggle(void) {
	LED_PORT ^= (1 << LED_PIN);
}
