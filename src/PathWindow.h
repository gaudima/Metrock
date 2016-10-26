#pragma once
#include <pebble.h>
#include "PathFinder.h"
#include "FBDraw.h"
#include "Util.h"

typedef struct path_node {
    int st;
    bool *s_cars;
    bool reverse;
} PathNode;

typedef struct path {
    int size;
    PathNode *nodes;
} Path;

Path path;

void open_pathview_window(char *time, char *minutes);
void free_path();
