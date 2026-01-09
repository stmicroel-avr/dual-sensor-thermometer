#include "init.h"
#include "utils/task_queue.h"
#include "tasks/display.h"
#include "tasks/led.h"
#include "tasks/temperature.h"
#include "tasks/rtc.h"
#include "tasks/pwr.h"
#include "tasks/temperature_log.h"

/**
 * Добавить все циклические задачи в очередь
 */
void add_loop_tasks(void) {
	// Блинк светодиода на старте
	task_queue_add_job(led_power_on);

	// Чтение и отображение температуры
	task_queue_add_job(print_temp_a);
	task_queue_add_job(print_temp_b);

	// Отображение RTC
	task_queue_add_job(display_rtc_time);

	// Отображение напряжения питания
	task_queue_add_job(display_pwr);

	// Обработчик записи лога
	task_queue_add_job(check_log_frame_set);

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