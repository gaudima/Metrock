#pragma once
#include <pebble.h>
#include "Structs.h"
#include "PathFinder.h"
#include "MainWindow.h"
#include "FBDraw.h"

int station_select_line;

void open_station_select_window(void (*selectStationCallback)(int, GRect));
void init_station_select_window();
void deinit_station_select_window();