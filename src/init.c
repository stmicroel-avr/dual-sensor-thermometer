#include "bsp/board.h"
#include "lib/ssd1306_oled/ssd1306xled.h"
#include "hal/hal_gpio.h"
#include "hal/hal_btn.h"
#include "lib/ds18b20/ds18b20_nb.h"
#include "lib/twi/twi.h"

/**
 * Инициализация всей периферии
 */
void init_all(void) {
	// Иницализация аппартного i2c
	twi_init();
	// Инициализация индикации
	hal_led_init();
	// Инициализация пина кнопки
	hal_btn_init();
	// Инициализация дисплея
	ssd1306_init();
	ssd1306_clear();
	// Инициализация датчиков температуры
	ds18b20_set_resolution(DS_A, DS_RESOLUTION);
	ds18b20_set_resolution(DS_B, DS_RESOLUTION);
}
