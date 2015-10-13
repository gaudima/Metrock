#pragma once

#include <pebble.h>

GRect interpolate_rect(GRect from, GRect to, AnimationProgress progress);

int strlen_utf8(char *s);

void custom_itoa(int num, char *buf);

void minutes_get_right(int num, char *buf);

