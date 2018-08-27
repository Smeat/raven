#ifndef COMPONENTS_HAL_ESP32_INCLUDE_HAL_DAC_H_
#define COMPONENTS_HAL_ESP32_INCLUDE_HAL_DAC_H_

#include <stdint.h>

void dac_hal_disable_channel(uint8_t channel);
void dac_hal_disable_all_channels();

#endif /* COMPONENTS_HAL_ESP32_INCLUDE_HAL_DAC_H_ */
