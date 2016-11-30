#pragma once

#include <pebble.h>

#define LOCALE_KEY_FROM 0
#define LOCALE_KEY_TO 1
#define LOCALE_KEY_LINE 2
#define LOCALE_KEY_MINUTE_PL_NOM 3
#define LOCALE_KEY_MINUTE_PL_GEN 4
#define LOCALE_KEY_MINUTE_SG 5
#define LOCALE_KEY_SCHEME_UPDATED 6

#define LOCALE_SIZE 7

char **locale;

void load_locale(uint32_t loc);
void free_locale();