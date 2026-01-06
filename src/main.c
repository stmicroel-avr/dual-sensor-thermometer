#include "init.h"
#include "utils/task_queue.h"
#include "handlers/display.h"
#include "handlers/led.h"
#include "handlers/temperature.h"
#include "handlers/button.h"
#include "handlers/rtc.h"
#include "handlers/pwr.h"

/**
 * Точка входа
 *
 * @return
 */
int main(void) {
	// Начальная инициализация
	init_all();

	// Добавить задачу по инициализации RTC
	task_queue_add_job(init_rtc_time);
	// Добавить задачи в очередь задач
	task_queue_add_job(show_startup_screen);
	// task_queue_add_job(check_btn_press);
	task_queue_add_job(led_power_on);
	//task_queue_add_job(delayed_unactive_sleep_screen);
	task_queue_add_job(print_temp_a);
	task_queue_add_job(print_temp_b);
	task_queue_add_job(display_pwr);
	task_queue_add_job(display_rtc_time);

	// Loop
	while (1) {
		task_queue_handle();
	}
}


