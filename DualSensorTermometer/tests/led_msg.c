/*
 * led_msg.c
 *
 * Created: 04.11.2025 18:50:11
 *  Author: Stepan
 */ 

#include "led_msg.h"



// Set durations and count of led blinks
void blinks(uint8_t duration_sec, uint8_t blinks) {
	uint32_t duration_ms = duration_sec * 1000UL;
	uint32_t half_blink_period_ms = duration_ms / (blinks * 2);
	
	hal_timer_start();
	
	uint32_t estimate_ms = 0;
	uint32_t start_ms = hal_ticks();
	uint32_t half_period_estimate_ms = start_ms;
	while(estimate_ms < duration_ms) {
		uint32_t ticks = hal_ticks();
		estimate_ms += (ticks - start_ms);
		start_ms = ticks;
		if ((ticks - half_period_estimate_ms) >= half_blink_period_ms) {
			hal_led_toggle();
			half_period_estimate_ms = ticks;
		}
	}
	
	hal_timer_stop();
}