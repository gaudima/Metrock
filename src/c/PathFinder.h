#pragma once
#include <pebble.h>
#include <limits.h>
#include "Structs.h"
#include "PathWindow.h"

typedef struct heap_node {
  int num;
  int weight;
} HeapNode;

typedef struct heap {
  int size;
  int realsize;
  HeapNode *h;
} Heap;

Heap *heap_create();

HeapNode *heap_get_min(Heap *what);

void heap_extract_min(Heap *what);

void heap_add_element(Heap *what, HeapNode node);

void heap_destroy(Heap *what);

int path_find(int from, int to);