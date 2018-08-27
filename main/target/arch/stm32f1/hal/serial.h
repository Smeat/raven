#ifndef COMPONENTS_HAL_ESP32_INCLUDE_HAL_UART_H_
#define COMPONENTS_HAL_ESP32_INCLUDE_HAL_UART_H_

#include <stdbool.h>

typedef void* uart_isr_handle_t; // TODO

#include <io/serial.h>

typedef struct serial_port_s serial_port_t;

void serial_hal_port_do_open(serial_port_t *port);
int serial_hal_port_read(serial_port_t *port, void *buf, size_t size, time_ticks_t timeout);
void serial_hal_port_close(serial_port_t *port);
bool serial_hal_port_set_baudrate(serial_port_t *port, uint32_t baudrate);
bool serial_hal_port_end_write(serial_port_t *port);
int serial_hal_port_write(serial_port_t *port, const void *buf, size_t size);
serial_port_t *serial_hal_port_open(const serial_port_config_t *config);
bool serial_hal_port_begin_write(serial_port_t *port);
bool serial_hal_port_is_half_duplex(serial_port_t *port);

#endif /* COMPONENTS_HAL_ESP32_INCLUDE_HAL_UART_H_ */
