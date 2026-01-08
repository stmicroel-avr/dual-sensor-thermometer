#include <stdint.h>
#include <stdbool.h>

#include "../hal/hal_gpio.h"

/**
 * Мигнуть светодиодом на старте
 *
 * @param elapsed_ms Тики таймера
 * @return
 */
bool led_power_on(uint32_t elapsed_ms) {
	static uint8_t toggles = 0;
	
	 if ((elapsed_ms / 250) > toggles) {
		 hal_led_toggle();
		 toggles++;
	 }
	
	 if (toggles >= 12) {
		 toggles = 0;
		 return false;
	 }

	 return true;
}