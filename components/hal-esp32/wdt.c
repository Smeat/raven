#include <esp_task_wdt.h>

#include <soc/timer_group_reg.h>
#include <soc/timer_group_struct.h>

void wdt_hal_enable() {
	ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
}

void wdt_hal_reset() {
	TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
	TIMERG0.wdt_feed = 1;
	TIMERG0.wdt_wprotect = 0;
}

