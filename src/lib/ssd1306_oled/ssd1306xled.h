#pragma once

#include <stdint.h>

// I2C адрес
#define SSD1306_SADDR   0x78

// ============== Low-level API ================== //

/**
 * Инициализация и конфигурирование контроллера дисплея
 */
void ssd1306_init(void);

/**
 * Установка позиции каретки дисплея
 *
 * @param x Координата горизонтали
 * @param page Координата вертикали (позиция фрейма символа)
 */
void ssd1306_setpos(uint8_t x, uint8_t page);

/**
 * Отрисовать символ на дисплее
 *
 * @param x Координата горизонтали
 * @param page Координата фрейма(вертикали)
 * @param c Символ
 */
void ssd1306_putc6x8(uint8_t x, uint8_t page, char c);

// =============== High level API ================ //

/**
 * Отрисовать строку на дисплее
 *
 * @param x Координата горизонтали
 * @param page Координата фрейма(вертикали)
 * @param s Строка
 */
void ssd1306_puts6x8(uint8_t x, uint8_t page, const char *s);

/**
 * Отправить команду на перевод дисплея в спящий режим
 */
void ssd1306_sleep(void);

/**
 * Отправить команду на пробуждение дисплея (из спящего)
 */
void ssd1306_wakeup(void);

/**
 * Очистка дисплея с конкретной строки
 * @param page Номер строки
 */
void ssd1306_clear_from_page(uint8_t page);

/**
 * Полная очистка дисплея
 */
void ssd1306_clear(void);