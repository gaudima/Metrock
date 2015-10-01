#include "Dither.h"

DitherPercentage getDitherFromPercentage(int percentage) {
    switch(percentage){
        case 0: return DITHER_0_PERCENT;
        case 10: return DITHER_10_PERCENT;
        case 20: return DITHER_20_PERCENT;
        case 25: return DITHER_25_PERCENT;
        case 30: return DITHER_30_PERCENT;
        case 40: return DITHER_40_PERCENT;
        case 50: return DITHER_50_PERCENT;
        case 60: return DITHER_60_PERCENT;
        case 70: return DITHER_70_PERCENT;
        case 75: return DITHER_75_PERCENT;
        case 80: return DITHER_80_PERCENT;
        case 90: return DITHER_90_PERCENT;
        case 100: return DITHER_100_PERCENT;
        default: return DITHER_0_PERCENT;
    }
}

int getPercentageFromDither(DitherPercentage dither) {
    switch(dither){

        case DITHER_0_PERCENT: return 0;
        case DITHER_10_PERCENT: return 10;
        case DITHER_20_PERCENT: return 20;
        case DITHER_25_PERCENT: return 25;
        case DITHER_30_PERCENT: return 30;
        case DITHER_40_PERCENT: return 40;
        case DITHER_50_PERCENT: return 50;

        case DITHER_60_PERCENT: return 60;
        case DITHER_70_PERCENT: return 70;
        case DITHER_75_PERCENT: return 75;
        case DITHER_80_PERCENT: return 80;
        case DITHER_90_PERCENT: return 90;
        case DITHER_100_PERCENT: return 100;

        default: return 0;
    }
}

static void draw_pixel(GBitmap *buffer, GPoint point, GColor color) {
    if(point.y >=0 && point.y <= 167 && point.x >= 0 && point.x <= 143) {
        uint8_t *data = gbitmap_get_data(buffer);
        uint16_t bpr = gbitmap_get_bytes_per_row(buffer);
        data[point.y * bpr + point.x] = color.argb;
    }
}

static void draw_zero_percent(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first) {
    for(int x = x_start; x < x_start+width; x++){
        for(int y = y_start; y < y_start+height; y++){
            draw_pixel(buffer, GPoint(x, y), first);
        }
    }
}

static void draw_ten_percent(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second) {
    for(int x = x_start; x < x_start+width; x++){
        for(int y = y_start; y < y_start+height; y++){
            if((x%8 == 0 && y%4 == 0) || ((x+4)%8 == 0 && (y+2)%4 == 0)) draw_pixel(buffer, GPoint(x, y), second);
            else draw_pixel(buffer, GPoint(x, y), first);
        }
    }
}

static void draw_twenty_percent(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second) {
    for(int x = x_start; x < x_start+width; x++){
        for(int y = y_start; y < y_start+height; y++){
            if((x%4 + y%4 == 0) || ((x+2)%4 + (y+2)%4 == 0)) draw_pixel(buffer, GPoint(x, y), second);
            else draw_pixel(buffer, GPoint(x, y), first);
        }
    }
}

static void draw_twenty_five_percent(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second){
    for(int x = x_start; x < x_start+width; x++){
        for(int y = y_start; y < y_start+height; y++){
            if((x%4 == 0 && y%2 == 0) || ((x+2)%4 == 0 && (y+1)%2 == 0)) draw_pixel(buffer, GPoint(x, y), second);
            else draw_pixel(buffer, GPoint(x, y), first);
        }
    }
}

static void draw_thirty_percent(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second){
    for(int x = x_start; x < x_start+width; x++){
        for(int y = y_start; y < y_start+height; y++){
            if( (x+y)%2 == 0 && ( (x+y)%4 != 0 || ( (x+y)%4 == 0 && x%2 != 0 && y%2 != 0) ) ) draw_pixel(buffer, GPoint(x, y), second);
            else draw_pixel(buffer, GPoint(x, y), first);
        }
    }
}

static void draw_forty_percent(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second){
    for(int x = x_start; x < x_start+width; x++){
        for(int y = y_start; y < y_start+height; y++){
            if( (x+y)%2 == 0 && ( (x+y)%8 != 0 || ( (x+y)%8 == 0 && x%4 != 1 && y%4 != 3 ) ) ) draw_pixel(buffer, GPoint(x, y), second);
            else draw_pixel(buffer, GPoint(x, y), first);
        }
    }
}

static void draw_fifty_percent(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second){
    for(int x = x_start; x < x_start+width; x++){
        for(int y = y_start; y < y_start+height; y++){
            if((x+y)%2 == 0) draw_pixel(buffer, GPoint(x, y), second);
            else draw_pixel(buffer, GPoint(x, y), first);
        }
    }
}

static void draw_dithered_rect_buffer(GBitmap *buffer, GRect bounds, GColor first_color, GColor second_color, DitherPercentage percentage){
    switch(percentage){
        case DITHER_0_PERCENT: draw_zero_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color); break;
        case DITHER_10_PERCENT: draw_ten_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;
        case DITHER_20_PERCENT: draw_twenty_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;
        case DITHER_25_PERCENT: draw_twenty_five_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;
        case DITHER_30_PERCENT: draw_thirty_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;
        case DITHER_40_PERCENT: draw_forty_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;
        case DITHER_50_PERCENT: draw_fifty_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;

        case DITHER_100_PERCENT: draw_zero_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, second_color); break;
        case DITHER_90_PERCENT: draw_ten_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, second_color, first_color); break;
        case DITHER_80_PERCENT: draw_twenty_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, second_color, first_color); break;
        case DITHER_75_PERCENT: draw_twenty_five_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, second_color, first_color); break;
        case DITHER_70_PERCENT: draw_thirty_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, second_color, first_color); break;
        case DITHER_60_PERCENT: draw_forty_percent(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, second_color, first_color); break;

        default: break;
    }
}

void draw_dithered_rect(GContext *ctx, GRect bounds, GColor first_color, GColor second_color, DitherPercentage percentage){
    GBitmap *buffer = graphics_capture_frame_buffer_format(ctx, GBitmapFormat8Bit);
    draw_dithered_rect_buffer(buffer, bounds, first_color, second_color, percentage);
    graphics_release_frame_buffer(ctx, buffer);
}


static void draw_top_to_bottom(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second){
    if(height >= 55){
        for(int i = 0; i < 110; i += 10){
            draw_dithered_rect_buffer(buffer, GRect(x_start, y_start+(i*height/110), width, 1+height/11), first, second, getDitherFromPercentage(i));
        }
    }
    else{
        for(int i = 0; i < 125; i += 25){
            draw_dithered_rect_buffer(buffer, GRect(x_start, y_start+(i*height/125), width, 1+height/5), first, second, getDitherFromPercentage(i));
        }
    }
}

static void draw_left_to_right(GBitmap *buffer, int x_start, int y_start, int width, int height, GColor first, GColor second){
    if(width >= 55){
        for(int i = 0; i < 110; i += 10){
            draw_dithered_rect_buffer(buffer, GRect(x_start+(i*width/110), y_start, 1+width/11, height), first, second, getDitherFromPercentage(i));
        }
    }
    else{
        for(int i = 0; i < 125; i += 25){
            draw_dithered_rect_buffer(buffer, GRect(x_start+(i*width/125), y_start, 1+width/5, height), first, second, getDitherFromPercentage(i));
        }
    }
}

static void draw_gradient_rect_buffer(GBitmap *buffer, GRect bounds, GColor first_color, GColor second_color, GradientDirection direction){
    switch(direction){

        case TOP_TO_BOTTOM: draw_top_to_bottom(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;
        case BOTTOM_TO_TOP: draw_gradient_rect_buffer(buffer, bounds, second_color, first_color, TOP_TO_BOTTOM); break;

        case LEFT_TO_RIGHT: draw_left_to_right(buffer, bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h, first_color, second_color); break;
        case RIGHT_TO_LEFT: draw_gradient_rect_buffer(buffer, bounds, second_color, first_color, LEFT_TO_RIGHT); break;

            //case TOP_LEFT_TO_BOTTOM_RIGHT: break;
            //case BOTTOM_RIGHT_TO_TOP_LEFT: draw_gradient_rect(ctx, bounds, second_color, first_color, TOP_LEFT_TO_BOTTOM_RIGHT); break;

            //case TOP_RIGHT_TO_BOTTOM_LEFT: break;
            //case BOTTOM_LEFT_TO_TOP_RIGHT: draw_gradient_rect(ctx, bounds, second_color, first_color, TOP_RIGHT_TO_BOTTOM_LEFT); break;

        default: break;
    }
}

void draw_gradient_rect(GContext *ctx, GRect bounds, GColor first_color, GColor second_color, GradientDirection direction){
    GBitmap *buffer = graphics_capture_frame_buffer_format(ctx, GBitmapFormat8Bit);
    draw_gradient_rect_buffer(buffer, bounds, first_color, second_color, direction);
    graphics_release_frame_buffer(ctx, buffer);
}

// find the distance between the two colors by determining the distance for each channel separately and use the greatest
static uint8_t get_color_distance(GColor first_color, GColor second_color) {
    uint8_t distance = 0;
    uint8_t dr = CHANNEL_DISTANCE(first_color.r, second_color.r);
    uint8_t dg = CHANNEL_DISTANCE(first_color.g, second_color.g);
    uint8_t db = CHANNEL_DISTANCE(first_color.b, second_color.b);
    if(dr > distance) distance = dr;
    if(dg > distance) distance = dg;
    if(db > distance) distance = db;

    // if the greatest distance is 2 and there is at least one distance 1, distance 3 looks better
    if(2 == distance) {
        if(1 == dr || 1 == dg || 1 == db) {
            distance = 3;
        }
    }
    return distance;
}


// get the value of a channel based for the current step
static uint8_t get_channel_for_step(uint8_t first, uint8_t second, uint8_t step) {
  uint8_t channel = first;
  if(first > second) {
    switch(first-second) {
      case 1: channel = (1 == step ? first : second); break;
      case 2: channel = first - 1; break;
      case 3: channel = first - step; break;
    }
  } else
  if(first < second) {
    switch(second-first) {
      case 1: channel = (1 == step ? first : second); break;
      case 2: channel = first + 1; break;
      case 3: channel = first + step; break;
    }
  }
  return channel;
}

// draw one smooth gradient rect consisting of up to 3 gradient rects
// the gradient looks smoother (most of the time) by calculating up to two colors in between the two specified colors
// if the color distance is 1, the result ist the same as a normal gradient rect
static void draw_smooth_gradient_rect_buffer(GBitmap *buffer, GRect bounds, GColor first_color, GColor second_color, GradientDirection direction) {
  uint8_t steps = get_color_distance(first_color, second_color);
  GColor gradient_colors[4] = {first_color, second_color, second_color, second_color};
  for(uint8_t i = 1; i < steps; ++i) {
    gradient_colors[i].r = get_channel_for_step(first_color.r, second_color.r, i);
    gradient_colors[i].g = get_channel_for_step(first_color.g, second_color.g, i);
    gradient_colors[i].b = get_channel_for_step(first_color.b, second_color.b, i);
  }

  switch(direction) {
    case TOP_TO_BOTTOM:
      for(uint8_t i = 0; i < steps; ++i) {
        draw_gradient_rect_buffer(buffer, GRect(bounds.origin.x, bounds.origin.y+((bounds.size.h/steps)*i), bounds.size.w, bounds.size.h/steps), gradient_colors[i], gradient_colors[i+1], TOP_TO_BOTTOM);
      }
      break;
    case BOTTOM_TO_TOP: draw_smooth_gradient_rect_buffer(buffer, bounds, second_color, first_color, TOP_TO_BOTTOM); break;

    case LEFT_TO_RIGHT:
      for(uint8_t i = 0; i < steps; ++i) {
        draw_gradient_rect_buffer(buffer, GRect(bounds.origin.x+((bounds.size.w/steps)*i), bounds.origin.y, bounds.size.w/steps, bounds.size.h), gradient_colors[i], gradient_colors[i+1], LEFT_TO_RIGHT);
      }
      break;
    case RIGHT_TO_LEFT: draw_smooth_gradient_rect_buffer(buffer, bounds, second_color, first_color, LEFT_TO_RIGHT); break;

    default: break;
  }
}

void draw_smooth_gradient_rect(GContext *ctx, GRect bounds, GColor first_color, GColor second_color, GradientDirection direction) {
    GBitmap *buffer = graphics_capture_frame_buffer_format(ctx, GBitmapFormat8Bit);
    draw_smooth_gradient_rect_buffer(buffer, bounds, first_color, second_color, direction);
    graphics_release_frame_buffer(ctx, buffer);
}