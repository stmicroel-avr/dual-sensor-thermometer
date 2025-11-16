#include <stdint.h>
#include <stdbool.h>

#include "../hal/hal_gpio.h"

// Handler of power on (blink twice)
bool led_power_on(uint32_t elapsed_ms) {
	static uint8_t toggles = 0;
	
	 if ((elapsed_ms / 250) > toggles) {
		 hal_led_toggle();
		 toggles++;
	 }
	
	 if (toggles >= 18) {
		 toggles = 0;
		 return false;
	 }

	 return true;
}