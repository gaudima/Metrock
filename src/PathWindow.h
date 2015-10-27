#pragma once
#include <pebble.h>
#include "PathFinder.h"
#include "FBDraw.h"
#include "Util.h"

typedef struct path {
    int size;
    int *nodes;
} Path;

Path *path;

void open_pathview_window(char *time, char *minutes);
