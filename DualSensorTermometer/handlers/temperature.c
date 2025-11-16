#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "temperature.h"
#include "../hal/hal_timer.h"
#include "../lib/display/ssd1306xled.h"
#include "../lib/temperature/ds18b20_nb.h"

#define TEMP_PERIOD_MS 1000u
#define CONV_MS_11BIT 375u
#define PAGE_A 1u
#define PAGE_B 2u
#define LINE_WIDTH 21u

static void render_temp_line(uint8_t page, const char *label, int16_t t16, bool indicator_on) {
	int neg = (t16 < 0);
	int16_t v = neg ? -t16 : t16;
	int whole = v / 16;
	int frac  = (v % 16) * 10 / 16;
	char *direction = neg ? "-" : "";
	char ind = indicator_on ? '*' : ' ';

	char tmp[LINE_WIDTH + 1];
	char line[LINE_WIDTH + 1];

	snprintf(tmp, sizeof(tmp), "%s %s%d.%d'C", label, direction, whole, frac);
	snprintf(line, sizeof(line), "%-20s%c", tmp, ind);

	ssd1306_puts6x8(0, page, line);
}

typedef enum { ST_START = 0, ST_WAIT, ST_READ, ST_GAP } ds_state_t;

static bool sensor_fsm_step(uint8_t pin_bit, uint8_t page, const char *label) {
	static struct {
		ds_state_t st_a, st_b;
		uint32_t   t0_a,  t0_b;
		bool       ind_a, ind_b;
	} s = { ST_START, ST_START, 0, 0, false, false };

	ds_state_t *st;
	uint32_t *t0;
	bool *indicator;
		
	if (page == PAGE_A) {
		st = &s.st_a;
		t0 = &s.t0_a;
		indicator = &s.ind_a;
		} else {
		st = &s.st_b;
		t0 = &s.t0_b;
		indicator = &s.ind_b;
	}

	switch (*st) {
		case ST_START:
			if (ds18b20_start(pin_bit)) {
				*t0 = hal_ticks();
				*st = ST_WAIT;
				} else {
				ssd1306_puts6x8(0, page, "Sensor ERR           ");
				*t0 = hal_ticks();
				*st = ST_GAP;
			}
			
			return true;
		case ST_WAIT:
			if (ds18b20_ready(pin_bit) || (hal_ticks() - *t0) >= CONV_MS_11BIT) {
				*st = ST_READ;
			}
				
			return true;
		case ST_READ: {
			int16_t t16;
			if (ds18b20_read_temp(pin_bit, &t16)) {
				*indicator = !(*indicator);
				render_temp_line(page, label, t16, *indicator);
				} else {
				ssd1306_puts6x8(0, page, "Read ERR             ");
			}
			*t0 = hal_ticks();
			*st = ST_GAP;
				
			return true;
		}

		case ST_GAP:
			if ((hal_ticks() - *t0) >= TEMP_PERIOD_MS) {
				*st = ST_START;
			}
			
			return true;
	}
		
	return true;
}

bool print_temp_a(uint32_t elapsed_ms) {
	(void)elapsed_ms;
	return sensor_fsm_step(DS_A, PAGE_A, "Temp1:");
}

bool print_temp_b(uint32_t elapsed_ms) {
	(void)elapsed_ms;
	return sensor_fsm_step(DS_B, PAGE_B, "Temp2:");
}
