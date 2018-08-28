#include "air/air.h"

#include "io/sx127x.h"

#include "air_radio_sx127x.h"

#define APPLY_ALL_RADIOS(function, radio, ...)      \
    for (int i = 0; i < RADIO_NUM; ++i)             \
    {                                               \
        function(&radio->sx127x[i], ##__VA_ARGS__); \
    }

void air_radio_init(air_radio_t *radio)
{
    APPLY_ALL_RADIOS(sx127x_init, radio);
}

void air_radio_set_tx_power(air_radio_t *radio, int dBm)
{
    APPLY_ALL_RADIOS(sx127x_set_tx_power, radio, dBm);
}

void air_radio_set_frequency(air_radio_t *radio, unsigned long freq, int error)
{
    APPLY_ALL_RADIOS(sx127x_set_frequency, radio, freq, error);
}

int air_radio_frequency_error(air_radio_t *radio)
{
    return sx127x_frequency_error(&radio->sx127x[0]); //TODO: add for all radios
}

void air_radio_set_sync_word(air_radio_t *radio, uint8_t word)
{
    APPLY_ALL_RADIOS(sx127x_set_lora_sync_word, radio, word);
}

void air_radio_start_rx(air_radio_t *radio)
{
    APPLY_ALL_RADIOS(sx127x_enable_continous_rx, radio);
}

void air_radio_set_mode(air_radio_t *radio, air_mode_e mode)
{
    APPLY_ALL_RADIOS(sx127x_idle, radio);

    APPLY_ALL_RADIOS(sx127x_set_lora_signal_bw, radio, SX127X_LORA_SIGNAL_BW_500);
    APPLY_ALL_RADIOS(sx127x_set_lora_header_mode, radio, SX127X_LORA_HEADER_IMPLICIT);
    APPLY_ALL_RADIOS(sx127x_set_lora_crc, radio, false);

    switch (mode)
    {
    case AIR_MODE_1:
        // We reduce coding rate in this mode to be as fast
        // as possible. Intended for short range and fast
        // update rate.
        APPLY_ALL_RADIOS(sx127x_set_lora_preamble_length, radio, 8);
        APPLY_ALL_RADIOS(sx127x_set_lora_spreading_factor, radio, 6);
        APPLY_ALL_RADIOS(sx127x_set_lora_coding_rate, radio, SX127X_LORA_CODING_RATE_4_5);
        break;
    case AIR_MODE_2:
        APPLY_ALL_RADIOS(sx127x_set_lora_preamble_length, radio, 6);
        APPLY_ALL_RADIOS(sx127x_set_lora_spreading_factor, radio, 7);
        APPLY_ALL_RADIOS(sx127x_set_lora_coding_rate, radio, SX127X_LORA_CODING_RATE_4_6);
        break;
    case AIR_MODE_3:
        APPLY_ALL_RADIOS(sx127x_set_lora_preamble_length, radio, 6);
        APPLY_ALL_RADIOS(sx127x_set_lora_spreading_factor, radio, 8);
        APPLY_ALL_RADIOS(sx127x_set_lora_coding_rate, radio, SX127X_LORA_CODING_RATE_4_6);
        break;
    case AIR_MODE_4:
        APPLY_ALL_RADIOS(sx127x_set_lora_preamble_length, radio, 6);
        APPLY_ALL_RADIOS(sx127x_set_lora_spreading_factor, radio, 9);
        APPLY_ALL_RADIOS(sx127x_set_lora_coding_rate, radio, SX127X_LORA_CODING_RATE_4_6);
        break;
    case AIR_MODE_5:
        APPLY_ALL_RADIOS(sx127x_set_lora_preamble_length, radio, 6);
        APPLY_ALL_RADIOS(sx127x_set_lora_spreading_factor, radio, 10);
        APPLY_ALL_RADIOS(sx127x_set_lora_coding_rate, radio, SX127X_LORA_CODING_RATE_4_8);
        break;
    }
}

void air_radio_set_bind_mode(air_radio_t *radio)
{
    sx127x_set_tx_power(&radio->sx127x, 1);
    sx127x_set_lora_sync_word(&radio->sx127x, SX127X_DEFAULT_LORA_SYNC_WORD);
    // Same as fast parameters as short range mode
    air_radio_set_mode(radio, AIR_MODE_FASTEST);
    sx127x_set_payload_size(&radio->sx127x, sizeof(air_bind_packet_t));
}

void air_radio_set_powertest_mode(air_radio_t *radio)
{
    air_radio_set_mode(radio, AIR_MODE_LONGEST);
    sx127x_set_lora_spreading_factor(&radio->sx127x, 12);
    sx127x_set_lora_signal_bw(&radio->sx127x, SX127X_LORA_SIGNAL_BW_250);
}

bool air_radio_is_tx_done(air_radio_t *radio)
{
    return sx127x_is_tx_done(&radio->sx127x);
}

bool air_radio_is_rx_done(air_radio_t *radio)
{
    return sx127x_is_rx_done(&radio->sx127x);
}

void air_radio_set_payload_size(air_radio_t *radio, size_t size)
{
    sx127x_set_payload_size(&radio->sx127x, size);
}

size_t air_radio_read(air_radio_t *radio, void *buf, size_t size)
{
    // TODO: shall we just read both radios to avoid losing any packets or switch based on rssi? Do we have to clear the buffer?
    return sx127x_read(radio->active, buf, size);
}

void air_radio_send(air_radio_t *radio, const void *buf, size_t size)
{
    // Only use the active radio to send!
    sx127x_send(radio->active, buf, size);
}

int air_radio_rssi(air_radio_t *radio, int *snr, int *lq)
{
    return sx127x_rssi(radio->active, snr, lq);
}

int air_radio_rssi_index(air_radio_t *radio, int *snr, int *lq, int radio_index)
{
    return sx127x_rssi(&radio->sx127x[radio_index], snr, lq);
}

void air_radio_set_active(air_radio_t *radio, int index)
{
    radio->active = &radio->sx127x[index];
}

int air_radio_get_count(air_radio_t *radio)
{
    return RADIO_NUM;
}

void air_radio_set_callback(air_radio_t *radio, air_radio_callback_t callback, void *callback_data)
{
    // XXX: only used in output. we should be fine by just using the active radio
    sx127x_set_callback(radio->active, callback, callback_data);
}

void air_radio_sleep(air_radio_t *radio)
{
    APPLY_ALL_RADIOS(sx127x_sleep, radio);
}

void air_radio_shutdown(air_radio_t *radio)
{
    APPLY_ALL_RADIOS(sx127x_shutdown, radio);
}

time_micros_t air_radio_full_cycle_time(air_radio_t *radio, air_mode_e mode)
{
    switch (mode)
    {
    case AIR_MODE_1:
        return MILLIS_TO_MICROS(10);
    case AIR_MODE_2:
        return MILLIS_TO_MICROS(20);
    case AIR_MODE_3:
        return MILLIS_TO_MICROS(33);
    case AIR_MODE_4:
        return MILLIS_TO_MICROS(57);
    case AIR_MODE_5:
        return MILLIS_TO_MICROS(115);
    }
    UNREACHABLE();
    return 0;
}

time_micros_t air_radio_uplink_cycle_time(air_radio_t *radio, air_mode_e mode)
{
    switch (mode)
    {
    case AIR_MODE_1:
        return MILLIS_TO_MICROS(4.5f);
    case AIR_MODE_2:
        return MILLIS_TO_MICROS(20);
    case AIR_MODE_3:
        return MILLIS_TO_MICROS(31);
    case AIR_MODE_4:
        return MILLIS_TO_MICROS(75);
    case AIR_MODE_5:
        return MILLIS_TO_MICROS(115);
    }
    UNREACHABLE();
    return 0;
}

bool air_radio_cycle_is_full(air_radio_t *radio, air_mode_e mode, unsigned seq)
{
#if 0
    // TODO
    if (mode == AIR_LORA_MODE_1)
    {
        return seq % 6 == 0;
    }
#endif
    return true;
}

time_micros_t air_radio_tx_failsafe_interval(air_radio_t *radio, air_mode_e mode)
{
    return air_radio_rx_failsafe_interval(radio, mode);
}

time_micros_t air_radio_rx_failsafe_interval(air_radio_t *radio, air_mode_e mode)
{
    switch (mode)
    {
    case AIR_MODE_1:
        return MILLIS_TO_MICROS(250);
    case AIR_MODE_2:
        return MILLIS_TO_MICROS(300);
    case AIR_MODE_3:
        return MILLIS_TO_MICROS(400);
    case AIR_MODE_4:
        return MILLIS_TO_MICROS(500);
    case AIR_MODE_5:
        return MILLIS_TO_MICROS(700);
    }
    UNREACHABLE();
    return 0;
}
