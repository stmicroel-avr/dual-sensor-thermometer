#include "../bsp/board.h"

/**
 *	Установка пина как вывода
 */
void hal_led_init(void) {
	LED_DDR |= (1 << LED_PIN);
}

/**
 *  Инвертировать значение регистра пина
 */
void hal_led_toggle(void) {
	LED_PORT ^= (1 << LED_PIN);
}