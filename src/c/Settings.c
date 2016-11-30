#include "Settings.h"

void load_settings() {
    if(persist_exists(PERSIT_KEY_SETTINGS)) {
        persist_read_data(PERSIT_KEY_SETTINGS, &settings, sizeof(Settings));
    } else {
        settings.city = RESOURCE_ID_SCHEME_MSK;
        settings.locale = RESOURCE_ID_LOCALE_EN;
        settings.scheme_locale = RESOURCE_ID_SCHEME_LOCALE_MSK_EN;
    }
}

void save_settings() {
    persist_write_data(PERSIT_KEY_SETTINGS, &settings, sizeof(Settings));
}