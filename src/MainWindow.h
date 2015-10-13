#pragma once
#include <pebble.h>
#include "Autogen.h"
#include "PathFinder.h"
#include "StationSelectWindow.h"
#include "Util.h"
#include "FBDraw.h"
#include "Dither.h"

typedef struct RectProps {
    GRect rect;
    GCornerMask corner_mask;
} RectProps;

void open_main_window();

void main_window_revert_back();
