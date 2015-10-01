#pragma once
#include <pebble.h>

#define CHANNEL_DISTANCE(X, Y) ((X)>(Y)?(X)-(Y):(Y)-(X))

typedef enum{
    TOP_TO_BOTTOM,
    BOTTOM_TO_TOP,
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT,
    TOP_LEFT_TO_BOTTOM_RIGHT,
    BOTTOM_RIGHT_TO_TOP_LEFT,
    TOP_RIGHT_TO_BOTTOM_LEFT,
    BOTTOM_LEFT_TO_TOP_RIGHT
} GradientDirection;

typedef enum{
    DITHER_0_PERCENT,
    DITHER_10_PERCENT,
    DITHER_20_PERCENT,
    DITHER_25_PERCENT,
    DITHER_30_PERCENT,
    DITHER_40_PERCENT,
    DITHER_50_PERCENT,
    DITHER_60_PERCENT,
    DITHER_70_PERCENT,
    DITHER_75_PERCENT,
    DITHER_80_PERCENT,
    DITHER_90_PERCENT,
    DITHER_100_PERCENT
} DitherPercentage;

DitherPercentage getDitherFromPercentage(int percentage);

int getPercentageFromDither(DitherPercentage dither);

void draw_dithered_rect(GContext *ctx, GRect bounds, GColor first_color, GColor second_color, DitherPercentage percentage);

void draw_gradient_rect(GContext *ctx, GRect bounds, GColor first_color, GColor second_color, GradientDirection direction);

void draw_smooth_gradient_rect(GContext *ctx, GRect bounds, GColor first_color, GColor second_color, GradientDirection direction);