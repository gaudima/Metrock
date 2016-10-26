#pragma once

#include <pebble.h>

void draw_filled_circle_buffer(GBitmap *buffer, GPoint center, int radius, GColor color);

void draw_line_buffer(GBitmap *buffer, GPoint p1, GPoint p2, GColor color);

void draw_filled_rect_buffer(GBitmap *buffer, GRect rect, GColor color);

void draw_gbitmap_buffer_2bpp(GBitmap *buffer, GBitmap *bitmap, GPoint top_left, GColor *palette);

void draw_separator(GBitmap *buffer, uint8_t y, GColor color);