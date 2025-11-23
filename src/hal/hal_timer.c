#include <avr/io.h>
#include <avr/interrupt.h>

// Глобальный статический счетчик тиков
static volatile uint32_t tick_counter = 0;

/**
 * Обработчик прерывания таймера
 */
ISR(TIMER0_COMPA_vect) {
	tick_counter++;
}

/**
 * Получить значение таймера
 *
 * @return uint32_t
 */
uint32_t hal_ticks(void) {
	uint32_t tmp;
	uint8_t sreg = SREG;
	cli();
	tmp = tick_counter;
	SREG = sreg;

	return tmp;
}

/**
 * Инициализация таймера
 */
void hal_timer_init(void) {
	TCCR0A = (1 << WGM01); // Timer/Counter0 инициализируем (Ставим бит WGM01 - сравнение и сброс)
	OCR0A  = 124; // Значение сброса таймера
	TIMSK0 = (1 << OCIE0A); // Разрешение прерывания по совпадению
	TCCR0B = 0; // Таймер настроен но остановлен
}

/**
 * Запуск таймера
 */
void hal_timer_start(void) {
	TCCR0B = (1 << CS01) | (1 << CS00);  // = 64
	sei(); // Разрешаем глобальные прерывания
}

/**
 * Остановка таймера
 */
void hal_timer_stop(void) {
	TCCR0B = 0;
}