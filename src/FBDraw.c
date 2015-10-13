#include "FBDraw.h"


static void draw_pixel(GBitmap *buffer, GPoint point, GColor color) {
    if(point.y >=0 && point.y <= 167 && point.x >= 0 && point.x <= 143) {
        uint8_t *data = gbitmap_get_data(buffer);
        uint16_t bpr = gbitmap_get_bytes_per_row(buffer);
        data[point.y * bpr + point.x] = color.argb;
    }
}

void draw_line_buffer(GBitmap *buffer, GPoint p1, GPoint p2, GColor color) {
    if(p1.x > p2.x) {
        GPoint tmp = p1;
        p1 = p2;
        p2 = tmp;
    }
    int deltax = abs(p2.x - p1.x);
    int deltay = abs(p2.y - p1.y);
    int error = 0;
    int deltaerr = deltay;
    int y = p1.y;
    for(int x = p1.x; x <= p2.x; x++) {
        draw_pixel(buffer, GPoint(x, y), color);
        error = error + deltaerr;
        if(2 * error >= deltax) {
            y = y - 1;
            error = error - deltax;
        }
    }
}

void draw_filled_circle_buffer(GBitmap *buffer, GPoint center, int radius, GColor color) {
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;

    while (y <= x) {
        draw_line_buffer(buffer,
                         GPoint(x + center.x, y + center.y),
                         GPoint(-x + center.x, y + center.y),
                         color);

        draw_line_buffer(buffer,
                         GPoint(y + center.x, x + center.y),
                         GPoint(-y + center.x, x + center.y),
                         color);

        draw_line_buffer(buffer,
                         GPoint(x + center.x, -y + center.y),
                         GPoint(-x + center.x, -y + center.y),
                         color);

        draw_line_buffer(buffer,
                         GPoint(y + center.x, -x + center.y),
                         GPoint(-y + center.x, -x + center.y),
                         color);
        y++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;
        }
        else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;
        }
    }
}

void draw_filled_rect_buffer(GBitmap *buffer, GRect rect, GColor color) {
    for(int x = rect.origin.x; x < rect.origin.x + rect.size.w; x++){
        for(int y = rect.origin.y; y < rect.origin.y + rect.size.h; y++){
            draw_pixel(buffer, GPoint(x, y), color);
        }
    }
}

void draw_separator(GBitmap *buffer, uint8_t y, GColor color) {
    for(int x = 0; x < 144; x+=2) {
        draw_pixel(buffer, GPoint(x, y), color);
    }
}