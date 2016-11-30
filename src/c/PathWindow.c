#include "PathWindow.h"

static Window *window;
static Layer *window_layer;
static Layer *graphics_layer;
static MenuLayer *menu_layer;
static char *time_str;
static char *minutes_str;

static GBitmap *train_head;
static GBitmap *train_car;

static GColor *palette_normal;
static GColor *palette_selected;

static uint16_t get_num_rows(struct MenuLayer *menu_layer, uint16_t section_index, void *callback_context) {
    return path.size;
}

static void draw_train_cars(GBitmap *buffer, GPoint top_left, bool *selected) {
    for(int i = 0; i < 5; i++) {
        GBitmap *img = train_car;
        GColor *palette = palette_normal;
        if(i == 0) {
            img = train_head;
        }
        if(selected[i]) {
            palette = palette_selected;
        }
        GRect img_bounds = gbitmap_get_bounds(img);
        draw_gbitmap_buffer_2bpp(buffer, img, GPoint(top_left.x + img_bounds.size.w * i + 20,
                                                     top_left.y + 44), palette);
    }
}

static void update_proc(Layer *this, GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, layer_get_frame(this), 0, GCornerNone);
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_draw_text(ctx, time_str, fonts_get_system_font(FONT_KEY_LECO_36_BOLD_NUMBERS),
                       GRect(0, -4, 144, 56), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    graphics_draw_text(ctx, minutes_str, fonts_get_system_font(FONT_KEY_GOTHIC_24),
                       GRect(0, 26, 144, 56), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    GBitmap *buf = graphics_capture_frame_buffer_format(ctx, GBitmapFormat8Bit);
    draw_separator(buf, 55, GColorBlack);
    graphics_release_frame_buffer(ctx, buf);
}

static int16_t menu_get_cell_height(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
    if(path.nodes[path.size - cell_index->row - 1].s_cars != NULL) {
        return 60;
    }
    return 44;
}

static void menu_draw_row(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context) {
    GBitmap *buffer = graphics_capture_frame_buffer_format(ctx, GBitmapFormat8Bit);
    GRect bounds = layer_get_frame(cell_layer);

    GPoint offset = scroll_layer_get_content_offset(menu_layer_get_scroll_layer(menu_layer));


    GRect frame = GRect(bounds.origin.x + offset.x, 56 + bounds.origin.y + offset.y, bounds.size.w, bounds.size.h);

    GColor line_color = pack->def->lines[pack->def->stations[pack->id2ind[path.nodes[path.size - cell_index->row - 1].st]].line].color;
    if(path.size != 1) {
        if (cell_index->row == 0) {
            draw_filled_rect_buffer(buffer,
                                    GRect(frame.origin.x + 5, frame.origin.y + 22, 9, frame.size.h - 22),
                                    GColorWhite);
            draw_filled_rect_buffer(buffer,
                                    GRect(frame.origin.x + 6, frame.origin.y + 22, 7, frame.size.h - 22),
                                    line_color);
        } else if (cell_index->row == path.size - 1) {
            draw_filled_rect_buffer(buffer,
                                    GRect(frame.origin.x + 5, frame.origin.y, 9, frame.size.h - 22),
                                    GColorWhite);
            draw_filled_rect_buffer(buffer,
                                    GRect(frame.origin.x + 6, frame.origin.y, 7, frame.size.h - 22),
                                    line_color);
        } else {
            draw_filled_rect_buffer(buffer,
                                    GRect(frame.origin.x + 5, frame.origin.y, 9, frame.size.h),
                                    GColorWhite);
            draw_filled_rect_buffer(buffer,
                                    GRect(frame.origin.x + 6, frame.origin.y, 7, frame.size.h),
                                    line_color);
        }
    }
    draw_filled_circle_buffer(buffer, GPoint(frame.origin.x + 9, frame.origin.y + 22), 7, GColorWhite);
    draw_filled_circle_buffer(buffer, GPoint(frame.origin.x + 9, frame.origin.y + 22), 6, line_color);

    if(path.nodes[path.size - cell_index->row - 1].s_cars != NULL) {
        draw_train_cars(buffer, GPoint(frame.origin.x, frame.origin.y), path.nodes[path.size - cell_index->row - 1].s_cars);
    }
    graphics_release_frame_buffer(ctx, buffer);

    if(menu_layer_get_selected_index(menu_layer).row == cell_index->row) {
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_fill_circle(ctx, GPoint(9, 22), 3);
    }
    GRect strect;
    if(strlen_utf8(pack->names[pack->def->stations[pack->id2ind[path.nodes[path.size - cell_index->row - 1].st]].name]) <= 15) {
        strect = GRect(20, 22 - 13, bounds.size.w - 23, bounds.size.h - 22);
    } else {
        strect = GRect(20, 0, bounds.size.w - 23, bounds.size.h);
    }
    graphics_draw_text(ctx,
                       pack->names[pack->def->stations[pack->id2ind[path.nodes[path.size - cell_index->row - 1].st]].name],
                       fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                       strect,
                       GTextOverflowModeTrailingEllipsis,
                       GTextAlignmentLeft,
                       NULL);
}

static void load(Window *win) {
    window_layer = window_get_root_layer(window);
    graphics_layer = layer_create(GRect(0, 0, 144, 56));
    menu_layer = menu_layer_create(GRect(0, 56, 144, 112));
    menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
            .draw_row = menu_draw_row,
            .get_num_rows = get_num_rows,
            .get_cell_height = menu_get_cell_height
    });
    menu_layer_pad_bottom_enable(menu_layer, false);
    menu_layer_set_highlight_colors(menu_layer, GColorWhite, GColorBlack);
    menu_layer_set_click_config_onto_window(menu_layer, window);
    layer_set_update_proc(graphics_layer, update_proc);
    layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
    layer_add_child(window_layer, graphics_layer);
    train_head = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TRAIN_HEAD);
    train_car = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TRAIN_CAR);
    palette_normal = malloc(sizeof(GColor) * 4);
    palette_normal[0] = GColorBlack;
    palette_normal[1].argb = 0b00000000;
    palette_normal[2] = GColorWhite;
    palette_normal[3].argb = 0b00000000;
    palette_selected = malloc(sizeof(GColor) * 4);
    palette_selected[0] = GColorBlack;
    palette_selected[1] = GColorBlack;
    palette_selected[2] = GColorRed;
    palette_selected[3].argb = 0b00000000;
}

static void unload(Window *win) {
    layer_destroy(graphics_layer);
    menu_layer_destroy(menu_layer);
    gbitmap_destroy(train_head);
    gbitmap_destroy(train_car);
    free(palette_normal);
    free(palette_selected);
}

void init_pathview_window() {
    window = window_create();
}

void deinit_pathview_window() {
    window_destroy(window);
}

void open_pathview_window(char *time, char *minutes) {
    minutes_str = minutes;
    time_str = time;
    window_set_window_handlers(window, (WindowHandlers) {
            .load = load,
            .unload = unload
    });
    window_stack_push(window, true);
}

void free_path() {
    if(path.nodes != NULL) {
        for (int i = 0; i < path.size; i++) {
            if (path.nodes[i].s_cars != NULL) {
                free(path.nodes[i].s_cars);
            }
        }
        free(path.nodes);
    }
}