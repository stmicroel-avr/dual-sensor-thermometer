#pragma once

#define SSD1306XLED_H

#include <stdint.h>

#define SSD1306_WIDTH    128
#define SSD1306_HEIGHT    32

// I2C address
#define SSD1306_SADDR   0x78

#define ssd1306_fill2(p1,p2) ssd1306_fill4(p1,p2,p1,p2)
void ssd1306_clear(void);

// Low-level API
void ssd1306_start_command(void);
void ssd1306_start_data(void);
void ssd1306_byte(uint8_t b);
void ssd1306_stop(void);

void ssd1306_init(void);
void ssd1306_setpos(uint8_t x, uint8_t page);
void ssd1306_fill(uint8_t pattern);
void ssd1306_fill4(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4);

// Print
void ssd1306_putc6x8(uint8_t x, uint8_t page, char c);
void ssd1306_puts6x8(uint8_t x, uint8_t page, const char *s);

// Control
void ssd1306_sleep(void);
void ssd1306_wakeup(void);