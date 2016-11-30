#pragma once

#include <pebble.h>

uint8_t* applyDelta(uint8_t* delta, size_t length, void (*readFn)(uint8_t *, size_t, size_t));