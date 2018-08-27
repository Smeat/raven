#include <stdio.h>

#include <hal/log.h>
#include <hal/wdt.h>
#include <hal/dac.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "target.h"

#include "air/air_radio.h"
#include "air/air_radio_sx127x.h"

#include "bluetooth/bluetooth.h"

#include "config/config.h"
#include "config/settings.h"
#include "config/settings_rmp.h"

#include "io/sx127x.h"

#include "p2p/p2p.h"

#include "platform/ota.h"
#include "platform/system.h"

#include "rc/rc.h"
#include "rc/rc_data.h"

#include "rmp/rmp.h"

#include "ui/ui.h"

#include "util/time.h"

static const char *TAG = "main";

static air_radio_t radio = {
    .sx127x.mosi = PIN_SX127X_MOSI,
    .sx127x.miso = PIN_SX127X_MISO,
    .sx127x.sck = PIN_SX127X_SCK,
    .sx127x.cs = PIN_SX127X_CS,
    .sx127x.rst = PIN_SX127X_RST,
    .sx127x.dio0 = PIN_SX127X_DIO0,
    .sx127x.output_type = SX127X_OUTPUT_TYPE,
};

static rc_t rc;
static rmp_t rmp;
#ifdef USE_P2P
static p2p_t p2p;
#endif
static ui_t ui;

static void shutdown(void)
{
    air_radio_shutdown(&radio);
    ui_shutdown(&ui);
    system_shutdown();
}

static void setting_changed(const setting_t *setting, void *user_data)
{
    if (SETTING_IS(setting, SETTING_KEY_POWER_OFF))
    {
        shutdown();
    }
}

void raven_ui_init(void)
{
    ui_config_t cfg = {
        .button = PIN_BUTTON_1,
#if defined(PIN_BUTTON_1_IS_TOUCH)
        .button_is_touch = true,
#else
        .button_is_touch = false,
#endif
        .beeper = PIN_BEEPER,
#ifdef USE_SCREEN
        .screen.sda = PIN_SCREEN_SDA,
        .screen.scl = PIN_SCREEN_SCL,
        .screen.rst = PIN_SCREEN_RST,
        .screen.addr = SCREEN_I2C_ADDR,
#endif
    };

    ui_init(&ui, &cfg, &rc);
}

void task_ui(void *arg)
{
    if (ui_screen_is_available(&ui))
    {
        ui_screen_splash(&ui);
    }

    for (;;)
    {
        ui_update(&ui);
        if (!ui_is_animating(&ui))
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}
#ifdef USE_P2P
void task_rmp(void *arg)
{
    p2p_start(&p2p);
    for (;;)
    {
        rmp_update(&rmp);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
#endif

void task_rc_update(void *arg)
{
    // Initialize the radio here so its interrupts
    // are fired in the same CPU as this task.
    air_radio_init(&radio);
    // Enable the WDT for this task
    wdt_hal_enable();
    for (;;)
    {
        rc_update(&rc);
        // Feed the WTD using the registers directly. Otherwise
        // we take too long here.
        wdt_hal_reset();
    }
}

void app_main()
{
    ota_init();

    config_init();
    settings_add_listener(setting_changed, NULL);

    // Disable DAC output on GPIO25 and GPIO26. It's enabled by default and
    // can alter the output levels otherwise.
    dac_hal_disable_all_channels();

    air_addr_t addr = config_get_addr();

    rmp_init(&rmp, &addr);

    settings_rmp_init(&rmp);
#ifdef USE_P2P
    p2p_init(&p2p, &rmp);
#endif

    rc_init(&rc, &radio, &rmp);

    raven_ui_init();

    xTaskCreatePinnedToCore(task_rc_update, "RC", 4096, NULL, 1, NULL, 1);
#ifdef USE_BLUETOOTH
    xTaskCreatePinnedToCore(task_bluetooh, "BLUETOOTH", 4096, &rc, 2, NULL, 0);
#endif
#ifdef USE_P2P
    xTaskCreatePinnedToCore(task_rmp, "RMP", 4096, NULL, 2, NULL, 0);
#endif

    // Start updating the UI after everything else is set up, since it queries other subsystems
    xTaskCreatePinnedToCore(task_ui, "UI", 4096, NULL, 1, NULL, 0);
}
