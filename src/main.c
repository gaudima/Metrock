#include <pebble.h>
#include "PathFinder.h"
#include "MainWindow.h"

void init() {
    APP_LOG(APP_LOG_LEVEL_INFO, "init");
    init_autogen();
    open_main_window();
}

void deinit() {
    deinit_autogen();
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
