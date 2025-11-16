#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ssd1306xled.h"
#include "font6x8.h"

static void twi_init(void) {
	TWSR = 0;
	TWBR = 2;
	TWCR = (1<<TWEN);
}

static void twi_start(uint8_t addr) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR = addr;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

static void twi_stop(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

static void twi_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

static void ssd1306_write_cmd(uint8_t cmd) {
	twi_start(SSD1306_SADDR);
	twi_write(0x00);     // control byte for command
	twi_write(cmd);
	twi_stop();
}

static void ssd1306_write_data(uint8_t data) {
	twi_start(SSD1306_SADDR);
	twi_write(0x40);     // control byte for data
	twi_write(data);
	twi_stop();
}

void ssd1306_init(void) {
	twi_init();

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

	// окно
	ssd1306_write_cmd(0x22); ssd1306_write_cmd(0x00); ssd1306_write_cmd(0x03);
	ssd1306_write_cmd(0x21); ssd1306_write_cmd(0x00); ssd1306_write_cmd(0x7F);

	ssd1306_write_cmd(0xAF);
	ssd1306_clear();
}

void ssd1306_setpos(uint8_t x, uint8_t page) {
	ssd1306_write_cmd(0xB0 | (page & 0x07));
	ssd1306_write_cmd(0x00 | (x & 0x0F));
	ssd1306_write_cmd(0x10 | ((x >> 4) & 0x0F));
}

void ssd1306_clear(void) {
	for (uint8_t page=0; page<4; page++) {
		ssd1306_setpos(0, page);
		for (uint8_t x=0; x<128; x++)
		ssd1306_write_data(0x00);
	}
}

void ssd1306_putc6x8(uint8_t x, uint8_t page, char c) {
	if (c < 32 || c > 127) c = '?';
	uint16_t idx = (c - 32) * 6;
	ssd1306_setpos(x, page);
	for (uint8_t i = 0; i < 6; i++)
	ssd1306_write_data(pgm_read_byte(&ssd1306xled_font6x8data[idx + i]));
}

void ssd1306_puts6x8(uint8_t x, uint8_t page, const char *s) {
	while (*s && x <= 122) {
		ssd1306_putc6x8(x, page, *s++);
		x += 6;
	}
}

void ssd1306_sleep(void) {
	ssd1306_write_cmd(0xAE);
}

void ssd1306_wakeup(void) {
	ssd1306_write_cmd(0xAF);
}
