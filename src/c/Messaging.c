#include <pebble.h>
#include "Messaging.h"
#include "Settings.h"
#include "UpdateWindow.h"
#include "Locales.h"
#include "Util.h"

static void message_received(DictionaryIterator *iter, void *context) {
    Tuple *city = dict_find(iter, MESSAGE_KEY_CITY_CHANGE);
    bool dirty = false;
    if(city) {
        freeSchemePack(pack);
        uint32_t c = city->value->uint32;
        DLOG("city change: %d", c);
        settings.city = RESOURCE_ID_SCHEME_MSK + c;
        dirty = true;
    }

    Tuple *loc = dict_find(iter, MESSAGE_KEY_LOCALE_CHANGE);
    if(loc) {
        free_locale();
        uint32_t l = loc->value->uint32;
        DLOG("locale change: %d", l);
        switch(settings.city) {
            case RESOURCE_ID_SCHEME_MSK: settings.scheme_locale = RESOURCE_ID_SCHEME_LOCALE_MSK_EN + l; break;
            case RESOURCE_ID_SCHEME_SPB: settings.scheme_locale = RESOURCE_ID_SCHEME_LOCALE_SPB_EN + l; break;
            case RESOURCE_ID_SCHEME_KIEV: settings.scheme_locale = RESOURCE_ID_SCHEME_LOCALE_KIEV_EN + l; break;
            case RESOURCE_ID_SCHEME_KHARKIV: settings.scheme_locale = RESOURCE_ID_SCHEME_LOCALE_KHARKIV_EN + l; break;
            case RESOURCE_ID_SCHEME_MINSK: settings.scheme_locale = RESOURCE_ID_SCHEME_LOCALE_MINSK_EN + l; break;
        }
        settings.locale = RESOURCE_ID_LOCALE_EN + l;
        dirty = true;
    }

    if(dirty) {
        loadSchemePack(settings.city, settings.scheme_locale);
        load_locale(settings.locale);
        save_settings();
        Window *top = window_stack_get_top_window();
        Layer *root = window_get_root_layer(top);
        layer_mark_dirty(root);
        open_update_window();
    }
}

void init_messaging() {
    app_message_register_inbox_received(message_received);
    app_message_open(128, 128);
}