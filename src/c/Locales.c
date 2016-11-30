#include "Locales.h"
#include "MsgPack.h"

void load_locale(uint32_t loc) {
    ResHandle locale_handle = resource_get_handle(loc);
    size_t len = resource_size(locale_handle);
    uint8_t* data = malloc(sizeof(uint8_t) * len);
    resource_load(locale_handle, data, len);
    locale = parseLocale(data);
    free(data);
}

void free_locale() {
    for(int i = 0; i < LOCALE_SIZE; i++) {
        free(locale[i]);
    }
    free(locale);
}