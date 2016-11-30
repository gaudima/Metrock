#pragma once

#include <pebble.h>

#define PERSIT_KEY_SETTINGS 0

typedef struct settings {
    uint32_t city;
    uint32_t locale;
    uint32_t scheme_locale;
} Settings;

Settings settings;

void load_settings();
void save_settings();