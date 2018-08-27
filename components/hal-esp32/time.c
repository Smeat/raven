#include <hal/time.h>


time_micros_t time_hal_micros_now()
{
	return esp_timer_get_time();
}
