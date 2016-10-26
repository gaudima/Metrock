#include <pebble.h>
#include "PathFinder.h"
#include "MainWindow.h"
#include "PathWindow.h"

void init() {
    APP_LOG(APP_LOG_LEVEL_INFO, "init");
    init_autogen();
    //path = malloc(sizeof(Path));
    //path->nodes = malloc(sizeof(int) * stationlen);
    open_main_window();
}

void deinit() {
    deinit_autogen();
    free(path.nodes);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
