#include <pebble.h>
#include "PathFinder.h"
#include "MainWindow.h"
#include "PathWindow.h"
#include "MsgPack.h"
#include "Structs.h"
#include "FossilDelta.h"
#include "Messaging.h"
#include "Settings.h"
#include "Locales.h"
#include "UpdateWindow.h"

void init() {
    APP_LOG(APP_LOG_LEVEL_INFO, "init");
    init_messaging();
    load_settings();
    loadSchemePack(settings.city, settings.scheme_locale);
    load_locale(settings.locale);
    init_main_window();
    init_station_select_window();
    init_pathview_window();
    init_update_window();
    open_main_window();
}

void deinit() {
    free_path();
    freeSchemePack(pack);
    free_locale();
    deinit_main_window();
    deinit_station_select_window();
    deinit_pathview_window();
    deinit_update_window();
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
