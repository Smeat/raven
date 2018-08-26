#include <assert.h>
#include <string.h>

#include <hal/mutex.h>
#include <hal/uart.h>

#include "io/pins.h"

#include "util/macros.h"

#include "serial.h"

void serial_port_do_open(serial_port_t *port)
{
    serial_hal_port_do_open(port);
}

serial_port_t *serial_port_open(const serial_port_config_t *config)
{
    return serial_hal_port_open(config);
}

int serial_port_read(serial_port_t *port, void *buf, size_t size, time_ticks_t timeout)
{
    return serial_port_read(port, buf, size, timeout);
}

bool serial_port_begin_write(serial_port_t *port)
{
    return serial_hal_port_begin_write(port);
}

bool serial_port_end_write(serial_port_t *port)
{
    return serial_hal_port_end_write(port);
}

int serial_port_write(serial_port_t *port, const void *buf, size_t size)
{
    return serial_hal_port_write(port, buf, size);
}

bool serial_port_set_baudrate(serial_port_t *port, uint32_t baudrate)
{
	return serial_hal_port_set_baudrate(port, baudrate);
}

void serial_port_close(serial_port_t *port)
{
    serial_hal_port_close(port);
}

bool serial_port_is_half_duplex(serial_port_t *port)
{
    return serial_hal_port_is_half_duplex(port);
}

void serial_port_destroy(serial_port_t **port)
{
    if (*port)
    {
        serial_port_close(*port);
        *port = NULL;
    }
}

io_flags_t serial_port_io_flags(serial_port_t *port)
{
    if (serial_port_is_half_duplex(port))
    {
        return IO_FLAG_HALF_DUPLEX;
    }
    return 0;
}
