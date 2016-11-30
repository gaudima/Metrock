#include "Structs.h"
#include "MsgPack.h"

static void freeLines(Line* lines, int len) {
    if(lines != NULL) {
        free(lines);
    }
}

static void freeTransfer(Transfer* transfer, int len) {
    if(transfer != NULL) {
        for (int i = 0; i < len; i++) {
            free(transfer[i].pos);
        }
        free(transfer);
    }
}

static void freeBoardInfo(BoardInfo *info) {
    if(info != NULL) {
        free(info->exit);
        freeTransfer(info->transfer, info->transferLen);
        free(info);
    }
}

static void freeStations(Station* stations, int len) {
    if(stations != NULL) {
        for (int i = 0; i < len; i++) {
            freeBoardInfo(stations[i].boardInfo);
        }
        free(stations);
    }
}

static void freeLinks(Link* links, int len) {
    if(links != NULL) {
        for (int i = 0; i < len; i++) {
            free(links[i].l);
        }
        free(links);
    }
}

static void freeScheme(Scheme* scheme) {
    if(scheme != NULL) {
        freeLines(scheme->lines, scheme->linesLen);
        freeStations(scheme->stations, scheme->stationsLen);
        freeLinks(scheme->links, scheme->linksLen);
        free(scheme);
    }
}

static void freeNames(char** names, int len) {
    if(names != NULL) {
        for (int i = 0; i < len; i++) {
            free(names[i]);
        }
        free(names);
    }
}

void loadSchemePack(uint32_t id, uint32_t loc) {
    ResHandle scheme = resource_get_handle(id);
    size_t len = resource_size(scheme);
    uint8_t* data = malloc(sizeof(uint8_t) * len);
    resource_load(scheme, data, len);
    pack = parseSchemePack(data);
    free(data);
    ResHandle loca = resource_get_handle(loc);
    len = resource_size(loca);
    data = malloc(sizeof(uint8_t) * len);
    resource_load(loca, data, len);
    pack->names = parseNames(data, &(pack->namesLen));
    free(data);
}

void freeSchemePack(SchemePack* pack) {
    if(pack != NULL) {
        freeScheme(pack->def);
        free(pack->id2ind);
        freeNames(pack->names, pack->namesLen);
        free(pack);
    }
}

