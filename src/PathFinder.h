#pragma once
#include <pebble.h>
#include <limits.h>
#include "Autogen.h"

typedef struct heap_node {
  int num;
  int weight;
} HeapNode;

typedef struct heap {
  int size;
  int realsize;
  HeapNode *h;
} Heap;

typedef struct path {
  int size;
  int *nodes;
} Path;

int path_from;

int path_to;

Heap *heap_create();

HeapNode *heap_get_min(Heap *what);

void heap_extract_min(Heap *what);

void heap_add_element(Heap *what, HeapNode node);

void heap_destroy(Heap *what);

void path_find_p(int from);

//Path *path_find(int from, int to);

//void path_destroy(Path *p);