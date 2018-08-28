#pragma once

#include "io/sx127x.h"

#define RADIO_NUM 1

typedef struct air_radio_s
{
    sx127x_t sx127x[RADIO_NUM];
    sx127x_t *active;
} air_radio_t;
