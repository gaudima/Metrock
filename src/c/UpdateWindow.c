#include "UpdateWindow.h"
#include "Locales.h"

static Window *window;
static Layer *window_layer;
static Layer *graphics_layer;
static Animation *anim;
static AppTimer *timer;
static GPoint left_start;
static GPoint left_end;
static GPoint right_start;
static GPoint right_end;
static int left_length;
static int right_length;

static void anim_update(Animation *animation, const AnimationProgress progress);

static AnimationImplementation anim_impl = {
        .update = anim_update,
};

static void timer_callback(void* data) {
    window_stack_pop(true);
}

static void anim_update(Animation *animation, const AnimationProgress progress) {
    int max_angle = TRIG_MAX_ANGLE / 8;

    int angle_left = TRIG_MAX_ANGLE / 4 + (max_angle * progress) / ANIMATION_NORMALIZED_MAX;
    int angle_right = TRIG_MAX_ANGLE / 4 - (max_angle * progress) / ANIMATION_NORMALIZED_MAX;
    int length_left = (left_length * progress) / ANIMATION_NORMALIZED_MAX;
    int length_right = (right_length * progress) / ANIMATION_NORMALIZED_MAX;
    left_end = GPoint(left_start.x + (cos_lookup(angle_left) * length_left) / TRIG_MAX_RATIO,
                      left_start.y - (sin_lookup(angle_left) * length_left) / TRIG_MAX_RATIO);

    right_end = GPoint(right_start.x + (cos_lookup(angle_right) * length_right) / TRIG_MAX_RATIO,
                       right_start.y - (sin_lookup(angle_right) * length_right) / TRIG_MAX_RATIO);
    layer_mark_dirty(graphics_layer);
}

static void update_proc(Layer *this, GContext *ctx) {
    graphics_context_set_stroke_width(ctx, 12);
    graphics_context_set_stroke_color(ctx, GColorIslamicGreen);
    graphics_draw_line(ctx, left_start, left_end);
    graphics_draw_line(ctx, right_start, right_end);
    GRect frame = layer_get_frame(this);
    graphics_context_set_text_color(ctx, GColorBlack);
    graphics_draw_text(ctx, locale[LOCALE_KEY_SCHEME_UPDATED], fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
                       GRect(0, frame.size.h / 2 + 20, 144, frame.size.h / 2 - 20),
                       GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

static void load(Window *win) {
    window_layer = window_get_root_layer(window);
    GRect frame = layer_get_frame(window_layer);
    graphics_layer = layer_create(frame);
    layer_set_update_proc(graphics_layer, update_proc);
    layer_add_child(window_layer, graphics_layer);
    anim = animation_create();
    animation_set_implementation(anim, &anim_impl);
    left_length = 30;
    right_length = 60;
    left_start = GPoint(frame.size.w / 2, frame.size.h / 2);
    right_start = GPoint(frame.size.w / 2, frame.size.h / 2);
    left_end = GPoint(frame.size.w / 2, frame.size.h / 2 - left_length);
    right_end = GPoint(frame.size.w / 2, frame.size.h / 2 - right_length);
    animation_set_duration(anim, preferred_result_display_duration() / 2);
    animation_set_curve(anim, AnimationCurveEaseOut);
    animation_schedule(anim);
    timer = app_timer_register(preferred_result_display_duration(), timer_callback, NULL);
}

static void unload(Window *win) {
    layer_destroy(graphics_layer);
}

void init_update_window() {
    window = window_create();
}

void deinit_update_window() {
    window_destroy(window);
}

void open_update_window() {
    window_set_window_handlers(window, (WindowHandlers) {
            .load = load,
            .unload = unload
    });
    window_stack_push(window, true);
}