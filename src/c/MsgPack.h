#pragma once

#include <pebble.h>
#include "Structs.h"

SchemePack* parseSchemePack(uint8_t *data);
char** parseNames(uint8_t *data, int* namesLen);
char** parseLocale(uint8_t *data);