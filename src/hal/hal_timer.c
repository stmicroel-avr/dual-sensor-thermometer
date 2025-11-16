#include <avr/io.h>
#include <avr/interrupt.h>

#include "hal_timer.h"

static volatile uint32_t tick_counter = 0;

ISR(TIMER0_COMPA_vect) {
	tick_counter++;
}

uint32_t hal_ticks(void) {
	uint32_t tmp;
	uint8_t sreg = SREG;
	cli();
	tmp = tick_counter;
	SREG = sreg;
	return tmp;
}

void hal_timer_init(void) {
	TCCR0A = (1 << WGM01);
	OCR0A  = 124;
	TIMSK0 = (1 << OCIE0A);
	TCCR0B = 0;
}

void hal_timer_start(void) {
	TCCR0B = (1 << CS01) | (1 << CS00);  // = 64
	sei();
}

void hal_timer_stop(void) {
	TCCR0B = 0;
}