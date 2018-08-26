#ifndef COMPONENTS_HAL_ESP32_INCLUDE_HAL_SYSTEM_H_
#define COMPONENTS_HAL_ESP32_INCLUDE_HAL_SYSTEM_H_

#include <stdbool.h>

float system_hal_temperature(void);
bool system_hal_awake_from_deep_sleep(void);
void system_hal_reboot(void);
void system_hal_shutdown(void);



#endif /* COMPONENTS_HAL_ESP32_INCLUDE_HAL_SYSTEM_H_ */
