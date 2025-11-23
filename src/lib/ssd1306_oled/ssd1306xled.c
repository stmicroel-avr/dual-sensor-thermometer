#include <avr/pgmspace.h>

#include "../twi/twi.h"
#include "ssd1306xled.h"
#include "font6x8.h"

/**
 * Отправить команду контроллеру дисплея
 *
 * @param cmd Команда
 */
static void ssd1306_write_cmd(uint8_t cmd) {
	twi_start(SSD1306_SADDR);
	twi_write(0x00);     // control byte for command
	twi_write(cmd);
	twi_stop();
}

/**
 * Отправить данные контроллеру дисплея
 *
 * @param data Данные
 */
static void ssd1306_write_data(uint8_t data) {
	twi_start(SSD1306_SADDR);
	twi_write(0x40);     // control byte for data
	twi_write(data);
	twi_stop();
}

/**
 * Инициализация и конфигурирование контроллера дисплея
 */
void ssd1306_init(void) {
	ssd1306_write_cmd(0xAE);
	ssd1306_write_cmd(0xD5); ssd1306_write_cmd(0xF0);
	ssd1306_write_cmd(0xA8); ssd1306_write_cmd(0x1F); // 128x32
	ssd1306_write_cmd(0xD3); ssd1306_write_cmd(0x00);
	ssd1306_write_cmd(0x40);
	ssd1306_write_cmd(0x8D); ssd1306_write_cmd(0x14);
	ssd1306_write_cmd(0x20); ssd1306_write_cmd(0x00);
	ssd1306_write_cmd(0xA0 | 0x01);
	ssd1306_write_cmd(0xC8);
	ssd1306_write_cmd(0xDA); ssd1306_write_cmd(0x02);
	ssd1306_write_cmd(0x81); ssd1306_write_cmd(0x3F);
	ssd1306_write_cmd(0xD9); ssd1306_write_cmd(0x22);
	ssd1306_write_cmd(0xDB); ssd1306_write_cmd(0x20);
	ssd1306_write_cmd(0xA4);
	ssd1306_write_cmd(0xA6);
	ssd1306_write_cmd(0x2E);

	ssd1306_write_cmd(0x22); ssd1306_write_cmd(0x00); ssd1306_write_cmd(0x03);
	ssd1306_write_cmd(0x21); ssd1306_write_cmd(0x00); ssd1306_write_cmd(0x7F);

	ssd1306_write_cmd(0xAF);
	ssd1306_clear();
}

/**
 * Установка позиции каретки дисплея
 *
 * @param x Координата горизонтали
 * @param page Координата вертикали (позиция фрейма символа)
 */
void ssd1306_setpos(uint8_t x, uint8_t page) {
	ssd1306_write_cmd(0xB0 | (page & 0x07));
	ssd1306_write_cmd(0x00 | (x & 0x0F));
	ssd1306_write_cmd(0x10 | ((x >> 4) & 0x0F));
}

/**
 * Отправить команды и данные для очистки дисплея
 */
void ssd1306_clear(void) {
	for (uint8_t page=0; page<4; page++) {
		ssd1306_setpos(0, page);
		for (uint8_t x=0; x<128; x++)
		ssd1306_write_data(0x00);
	}
}


/**
 * Отрисовать символ на дисплее
 *
 * @param x Координата горизонтали
 * @param page Координата фрейма(вертикали)
 * @param c Символ
 */
void ssd1306_putc6x8(uint8_t x, uint8_t page, char c) {
	uint16_t idx = (c - 32) * 6;
	ssd1306_setpos(x, page);
	for (uint8_t i = 0; i < 6; i++) {
		ssd1306_write_data(pgm_read_byte(&ssd1306xled_font6x8data[idx + i]));
	}
}

/**
 * Отрисовать строку на дисплее
 *
 * @param x Координата горизонтали
 * @param page Координата фрейма(вертикали)
 * @param s Строка
 */
void ssd1306_puts6x8(uint8_t x, uint8_t page, const char *s) {
	while (*s && x <= 122) {
		ssd1306_putc6x8(x, page, *s++);
		x += 6;
	}
}

/**
 * Отправить команду на перевод дисплея в спящий режим
 */
void ssd1306_sleep(void) {
	ssd1306_write_cmd(0xAE);
}

/**
 * Отправить команду на пробуждение дисплея (из спящего)
 */
void ssd1306_wakeup(void) {
	ssd1306_write_cmd(0xAF);
}
