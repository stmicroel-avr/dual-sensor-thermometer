#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "temperature.h"
#include "../hal/hal_timer.h"
#include "../lib/ssd1306_oled/ssd1306xled.h"
#include "../lib/ds18b20/ds18b20_nb.h"

// Целевой период обновления температуры
#define TEMP_PERIOD_MS 500u
// Длина строки
#define LINE_WIDTH 21u
// Структура для хранения стейта считывания данных с датчика
typedef enum { ST_START = 0, ST_WAIT, ST_READ, ST_GAP } ds_state_t;

/**
 * Рендер строки отображения строки температуры
 *
 * @param page Строка
 * @param label Префикс(Статическая часть)
 * @param t16 Значение температуры в int16
 * @param indicator_on Отображать индикатор в конце строки (*)
 */
static void render_temp_line(uint8_t page, const char *label, int16_t t16, bool indicator_on) {
	int is_neg = t16 < 0;
	int16_t m_value = is_neg ? -t16 : t16;
	const char *direction = is_neg ? "-" : "";

	int16_t t_int  = m_value / 16;               // целая
	int16_t t_frac = (m_value % 16) * 100 / 16;  // сотые

	char tmp[LINE_WIDTH + 1];
	char line[LINE_WIDTH + 1];

	snprintf(tmp, sizeof(tmp), "%s %s%d.%02d'C", label, direction, t_int, t_frac);
	snprintf(line, sizeof(line), "%-20s%c", tmp, indicator_on ? '*' : ' ');

	ssd1306_puts6x8(0, page, line);
}


/**
 * Обработка этапа считывания информации с датчика
 *
 * @param pin_bit Пин порта датчика
 * @param page Номер строки
 * @param label Префикс строки
 * @return
 */
void sensor_fsm_step(uint8_t pin_bit, uint8_t page, const char *label) {
	static struct {
		ds_state_t st_a, st_b;
		uint32_t t0_a,  t0_b;
		bool ind_a, ind_b;
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
			break;
		case ST_WAIT:
			if (ds18b20_ready(pin_bit)) {
				*st = ST_READ;
			}
			break;
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
			break;
		}
		case ST_GAP:
			if ((hal_ticks() - *t0) >= TEMP_PERIOD_MS) {
				*st = ST_START;
			}
			break;
	}
}
