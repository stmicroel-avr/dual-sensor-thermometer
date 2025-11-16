#include "../bsp/board.h"

// Initialize led pin as output
void hal_led_init(void) {
	LED_DDR |= (1 << LED_PIN);
}

// Invert pin value
void hal_led_toggle(void) {
	LED_PORT ^= (1 << LED_PIN);
}
