#include "init.h"
#include "utils/task_queue.h"
#include "handlers/display.h"
#include "handlers/led.h"
#include "handlers/temperature.h"
#include "handlers/rtc.h"
#include "handlers/pwr.h"

/**
 * Добавить все циклические задачи в очередь
 */
void add_loop_tasks(void) {
	// Блинк светодиода на старте
	task_queue_add_job(led_power_on);

	// Чтение и отображение температуры
	task_queue_add_job(print_temp_a);
	task_queue_add_job(print_temp_b);

	// Отображение напряжения питания
	task_queue_add_job(display_pwr);

	// Отображение RTC
	task_queue_add_job(display_rtc_time);

	// Отключенные временно таски
	// task_queue_add_job(check_btn_press);
	// task_queue_add_job(delayed_unactive_sleep_screen);
}

/**
 * Точка входа
 *
 * @return
 */
int main(void) {
	// Начальная инициализация
	init_all();

	// Инициализация RTC
	init_rtc_time();

	// Показать начальный экран
	show_startup_screen();

	// Инициализация очереди
	task_queue_init();

	// Добавить все циклические задачи в очередь
	add_loop_tasks();

	// Запуск циклических задач
	while (1) {
		task_queue_handle();
	}
}