#include "MainWindow.h"
#include "Locales.h"

static Window *window;
static Layer *window_layer;
static Layer *graphics_layer;
static Animation *animation;
static bool to_animation_playing;
static bool from_animation_playing;

static GRect to_rect;
static GRect from_rect;
static GRect to_rect_final;
static GRect from_rect_final;

static char time_str[5];
static int time_int;

static char minutes_str[13];

static int path_from;
static int path_to;

static void select_from_callback(int index, GRect final) {
    path_from = index;
    time_int = path_find(path_from, path_to) / 60;
    from_rect_final = final;
    layer_mark_dirty(graphics_layer);
    main_window_revert_back();
}

static void select_to_callback(int index, GRect final) {
    path_to = index;
    time_int = path_find(path_from, path_to) / 60;
    to_rect_final = final;
    layer_mark_dirty(graphics_layer);
    main_window_revert_back();
}

static void update_from_animation(Animation *anim, const AnimationProgress progress) {
    from_rect = interpolate_rect(GRect(128, 25, 5, 5), from_rect_final, progress);
    layer_mark_dirty(graphics_layer);
}

static void update_fromback_animation(Animation *anim, const AnimationProgress progress) {
    from_rect = interpolate_rect(from_rect_final, GRect(128, 25, 5, 5), progress);
    layer_mark_dirty(graphics_layer);
}

static void update_to_animation(Animation *anim, const AnimationProgress progress) {
    to_rect = interpolate_rect(GRect(128, 137, 5, 5), to_rect_final, progress);
    layer_mark_dirty(graphics_layer);
}

static void update_toback_animation(Animation *anim, const AnimationProgress progress) {
    to_rect = interpolate_rect(to_rect_final, GRect(128, 137, 5, 5), progress);
    layer_mark_dirty(graphics_layer);
}

static void animation_teardown(Animation *anim) {
    to_animation_playing = false;
    from_animation_playing = false;
}

static AnimationImplementation to_implementation = {
        .update = update_to_animation
};

static AnimationImplementation from_implementation = {
        .update = update_from_animation
};

static AnimationImplementation toback_implementation = {
        .update = update_toback_animation,
        .teardown = animation_teardown
};

static AnimationImplementation fromback_implementation = {
        .update = update_fromback_animation,
        .teardown = animation_teardown
};

void main_window_revert_back() {
    animation = animation_create();
    if(to_animation_playing) {
        animation_set_implementation(animation, &toback_implementation);
    } else if(from_animation_playing) {
        animation_set_implementation(animation, &fromback_implementation);
    }
    animation_schedule(animation);
}
static void reset_animations() {
    from_rect = GRect(128, 25, 5, 5);
    to_rect = GRect(128, 137, 5, 5);
    from_rect_final = GRect(0, 0, 144, 168);
    to_rect_final = GRect(0, 0, 144, 168);
    to_animation_playing = false;
    from_animation_playing = false;
}

static void open_station_select_from_callback(Animation *anim, bool finished, void *context) {
    open_station_select_window(select_from_callback);
}

static void open_station_select_to_callback(Animation *anim, bool finished, void *context) {
    open_station_select_window(select_to_callback);
}

static void select_to(ClickRecognizerRef recognizer, void *context) {
    reset_animations();
    animation = animation_create();
    animation_set_implementation(animation, &to_implementation);
    animation_set_handlers(animation, (AnimationHandlers) {.stopped = open_station_select_to_callback}, NULL);
    to_animation_playing = true;
    animation_schedule(animation);
    station_select_line = pack->def->stations[pack->id2ind[path_to]].line; //stations[graph_index[path_to]].line;
}

static void open_path_wnd(ClickRecognizerRef recognizer, void *context) {
    open_pathview_window(time_str, minutes_str);
}

static void swap_sations_animate(ClickRecognizerRef recognizer, void *context) {

}

static void swap_stations(ClickRecognizerRef recognizer, void *context) {
    int tmp = path_from;
    path_from = path_to;
    path_to = tmp;
    time_int = path_find(path_from, path_to) / 60;
    layer_mark_dirty(graphics_layer);
}

static void select_from(ClickRecognizerRef recognizer, void *context) {
    reset_animations();
    animation = animation_create();
    animation_set_implementation(animation, &from_implementation);
    animation_set_handlers(animation, (AnimationHandlers) {.stopped = open_station_select_from_callback}, NULL);
    from_animation_playing = true;
    animation_schedule(animation);
    station_select_line = pack->def->stations[pack->id2ind[path_from]].line;
}

static void click_config_provider(void *context) {
    window_long_click_subscribe(BUTTON_ID_SELECT, 300, NULL, swap_stations);
    window_single_click_subscribe(BUTTON_ID_SELECT, open_path_wnd);
    window_single_click_subscribe(BUTTON_ID_UP, select_from);
    window_single_click_subscribe(BUTTON_ID_DOWN, select_to);
}

static void update_proc(Layer *this, GContext *ctx) {
    if(!to_animation_playing) {
        graphics_context_set_fill_color(ctx, pack->def->lines[pack->def->stations[pack->id2ind[path_from]].line].color);
        graphics_fill_circle(ctx, GPoint(130, 27), 6);
        graphics_fill_rect(ctx, from_rect, 0, GCornerNone);
    }

    if(!from_animation_playing) {
        graphics_context_set_fill_color(ctx, pack->def->lines[pack->def->stations[pack->id2ind[path_to]].line].color);
        graphics_fill_circle(ctx, GPoint(130, 139), 6);
        graphics_fill_rect(ctx, to_rect, 0, GCornerNone);
    }

    if(!to_animation_playing && !from_animation_playing) {
        GBitmap *fb = graphics_capture_frame_buffer_format(ctx, GBitmapFormat8Bit);
        draw_separator(fb, 54, GColorBlack);
        draw_separator(fb, 112, GColorBlack);
        graphics_release_frame_buffer(ctx, fb);
        graphics_context_set_text_color(ctx, GColorBlack);

        custom_itoa(time_int, time_str);
        minutes_get_right(time_int, minutes_str);

        graphics_draw_text(ctx, time_str, fonts_get_system_font(FONT_KEY_LECO_36_BOLD_NUMBERS),
                           GRect(0, 52, 144, 56), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, minutes_str, fonts_get_system_font(FONT_KEY_GOTHIC_24),
                           GRect(0, 82, 144, 56), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, locale[LOCALE_KEY_FROM], fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(4, 0, 132, 26),
                           GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
        graphics_draw_text(ctx, locale[LOCALE_KEY_TO], fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(4, 112, 132, 26),
                           GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
        graphics_draw_text(ctx, pack->names[pack->def->stations[pack->id2ind[path_from]].name], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                           GRect(4, 14, 116, 44), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
        graphics_draw_text(ctx, pack->names[pack->def->stations[pack->id2ind[path_to]].name], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
                           GRect(4, 126, 116, 44), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
    }
}

static void load(Window *win) {
    path_from = 0;
    path_to = 1;
    path.nodes = NULL;
    time_int = path_find(path_from, path_to) / 60;
    reset_animations();
    window_layer = window_get_root_layer(window);
    GRect frame = layer_get_frame(window_layer);
    graphics_layer = layer_create(frame);
    layer_set_update_proc(graphics_layer, update_proc);
    layer_add_child(window_layer, graphics_layer);
    window_set_click_config_provider(window, click_config_provider);
}

static void unload(Window *win) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "unload");
    layer_destroy(graphics_layer);
}

void init_main_window() {
    window = window_create();
}

void deinit_main_window() {
    window_destroy(window);
}

void open_main_window() {
    window_set_window_handlers(window, (WindowHandlers) {
            .load = load,
            .unload = unload
    });
    window_stack_push(window, true);
}