/*
 * led.c
 *
 * Created: 05.11.2025 21:34:45
 *  Author: Stepan
 */ 

#include <stdint.h>
#include <stdbool.h>
#include "../hal/hal_gpio.h"


// Handler of power on (blink twice)
bool led_power_on(uint32_t elapsed_ms) {
	static uint8_t toggles = 0;
	
	 if ((elapsed_ms / 500) > toggles) {
		 hal_led_toggle();
		 toggles++;
	 }
	
	 if (toggles >= 16) {
		 toggles = 0;
		 return false;
	 }

	 return true;
}