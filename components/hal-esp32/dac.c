#include <hal/dac.h>

#include <driver/gpio.h>
#include <driver/dac.h>


void dac_hal_disable_channel(uint8_t channel){
	ESP_ERROR_CHECK(dac_output_disable(channel));
}

void dac_hal_disable_all_channels(){
	dac_hal_disable_channel(1);
	dac_hal_disable_channel(2);
}



