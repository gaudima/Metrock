#pragma once
#include <pebble.h>
#include "Autogen.h"
#include "PathFinder.h"
#include "StationSelectWindow.h"

typedef struct RectProps {
    GRect rect;
    GCornerMask corner_mask;
} RectProps;

void open_main_window();

void main_window_revert_back();
