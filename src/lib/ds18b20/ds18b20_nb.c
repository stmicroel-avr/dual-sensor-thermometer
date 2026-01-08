#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../../board/board.h"
#include <util/delay.h>

// ===================== 1-Wire low-level (PORTD only) =====================

static void ow_line_low(uint8_t bit) {
	PORTD &= ~(1 << bit);
	DDRD |= 1 << bit;
}

static void ow_line_release(uint8_t bit) {
	DDRD &= ~(1 << bit);
}

static uint8_t ow_sample(uint8_t bit) {
	return (PIND & (1 << bit)) ? 1 : 0;
}

// reset: presence pulse detect
static bool ow_reset(uint8_t bit) {
	uint8_t sreg = SREG; cli();

	ow_line_low(bit);
	_delay_us(480);
	ow_line_release(bit);
	_delay_us(70);
	bool present = (ow_sample(bit) == 0);
	_delay_us(410);

	SREG = sreg;
	return present;
}

// write single bit
static void ow_write_bit(uint8_t bit, uint8_t v) {
	uint8_t sreg = SREG; 
	cli();

	if (v) {
		// write '1'
		ow_line_low(bit);
		_delay_us(6);
		ow_line_release(bit);
		_delay_us(64);
	} else {
		// write '0'
		ow_line_low(bit);
		_delay_us(60);
		ow_line_release(bit);
		_delay_us(10);
	}

	SREG = sreg;
}

// read single bit
static uint8_t ow_read_bit(uint8_t bit) {
	uint8_t sreg = SREG; cli();

	ow_line_low(bit);
	_delay_us(6);
	ow_line_release(bit);
	_delay_us(9);
	uint8_t v = ow_sample(bit);
	_delay_us(55);

	SREG = sreg;
	return v;
}

static void ow_write_byte(uint8_t bit, uint8_t byte) {
	for (uint8_t i = 0; i < 8; i++) {
		ow_write_bit(bit, byte & 0x01);
		byte >>= 1;
	}
}

static uint8_t ow_read_byte(uint8_t bit) {
	uint8_t r = 0;
	for (uint8_t i = 0; i < 8; i++) {
		r >>= 1;
		if (ow_read_bit(bit)) {
			r |= 0x80;
		}
	}

	return r;
}

// ===================== DS18B20 high-level (non-blocking) =====================

#define DS_SKIP_ROM      0xCC
#define DS_CONVERT_T     0x44
#define DS_READ_SCRATCH  0xBE
#define DS_WRITE_SCRATCH 0x4E
#define DS_COPY_SCRATCH  0x48

void ds18b20_set_resolution(uint8_t bit, uint8_t res_bits) {
	if (res_bits < 9) {
		res_bits = 9;
	}

	if (res_bits > 12) {
		res_bits = 12;
	}

	uint8_t cfg = ((res_bits - 9) << 5) | 0x1F;

	if (!ow_reset(bit)) {
		return;
	}

	ow_write_byte(bit, DS_SKIP_ROM);
	ow_write_byte(bit, DS_WRITE_SCRATCH);
	ow_write_byte(bit, 0x4B);
	ow_write_byte(bit, 0x46);
	ow_write_byte(bit, cfg);
}

bool ds18b20_start(uint8_t bit) {
	if (!ow_reset(bit)) {
		return false;
	}

	ow_write_byte(bit, DS_SKIP_ROM);
	ow_write_byte(bit, DS_CONVERT_T);

	return true;
}

bool ds18b20_ready(uint8_t bit) {
	return ow_read_bit(bit) ? true : false;
}

bool ds18b20_read_temp(uint8_t bit, int16_t *t) {
	if (!ow_reset(bit)) {
		return false;
	}

	ow_write_byte(bit, DS_SKIP_ROM);
	ow_write_byte(bit, DS_READ_SCRATCH);
	uint8_t tempL = ow_read_byte(bit);
	uint8_t tempH = ow_read_byte(bit);
	
	for (uint8_t i = 0; i < 7; i++) {
		(void)ow_read_byte(bit);
	}
	
	int16_t raw = (int16_t)((tempH << 8) | tempL);
	if (t) {
		*t = raw;
	}

	return true;
}
