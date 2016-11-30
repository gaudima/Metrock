#pragma once
#include <pebble.h>

typedef struct link2 {
    int to;
    int weight;
} Link2;

typedef struct link {
    int len;
    Link2* l;
} Link;

typedef struct transfer {
    int prevStation;
    int toStation;
    int nextStation;
    int* pos;
} Transfer;

typedef struct boardInfo {
    int* exit;
    int transferLen;
    Transfer *transfer;
} BoardInfo;

typedef struct station {
    int id;
    int line;
    int name;
    BoardInfo* boardInfo;
} Station;

typedef struct line {
    int name;
    GColor color;
    int startFrom;
    int stations;
} Line;

typedef struct scheme {
    int linesLen;
    Line* lines;
    int stationsLen;
    Station* stations;
    int linksLen;
    Link* links;
} Scheme;

typedef struct schemePack {
    int id;
    int ver;
    Scheme* def;
    int* id2ind;
    int namesLen;
    char** names;
} SchemePack;

void loadSchemePack(uint32_t id, uint32_t locale);
void freeSchemePack(SchemePack *pack);

SchemePack *pack;
