#include "PathFinder.h"

static int *d;
static int *p;
static Heap *q;

static void heap_sift_up(Heap *what, int i) {
    while (what->h[i].weight < what->h[(i - 1) / 2].weight) {
        HeapNode tmp;
        tmp = what->h[i];
        what->h[i] = what->h[(i - 1) / 2];
        what->h[(i - 1) / 2] = tmp;
        i = (i - 1) / 2;
    }
}

static void heap_sift_down(Heap *what, int i) {
    while (2 * i + 1 < what->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int j = left;
        if (right < what->size && what->h[right].weight < what->h[left].weight) {
            j = right;
        }
        if (what->h[i].weight <= what->h[j].weight) {
            break;
        }
        HeapNode tmp;
        tmp = what->h[i];
        what->h[i] = what->h[j];
        what->h[j] = tmp;
        i = j;
    }
}

Heap *heap_create() {
    Heap *ret = malloc(sizeof(Heap));
    ret->size = 0;
    ret->realsize = 1;
    ret->h = malloc(sizeof(HeapNode));
    return ret;
}

HeapNode *heap_get_min(Heap *what) {
    return what->h;
}

void heap_extract_min(Heap *what) {
    what->h[0] = what->h[what->size - 1];
    what->size -= 1;
    heap_sift_down(what, 0);
}

void heap_add_element(Heap *what, HeapNode node) {
    if (what->realsize < what->size + 1) {
        what->realsize *= 2;
        what->h = realloc(what->h, what->realsize * sizeof(HeapNode));
    }
    what->h[what->size] = node;
    what->size += 1;
    heap_sift_up(what, what->size - 1);
}

void heap_destroy(Heap *what) {
    free(what->h);
    free(what);
}

int path_find(int from, int to) {
    d = malloc(stationlen * sizeof(int));
    p = malloc(stationlen * sizeof(int));
    for (int i = 0; i < stationlen; i++) {
        d[i] = INT_MAX;
        p[i] = 0;
    }
    d[from] = 0;
    q = heap_create();
    heap_add_element(q, (HeapNode) {.weight = 0, .num = from});
    while (q->size != 0) {
        int v = heap_get_min(q)->num;
        int cur_d = heap_get_min(q)->weight;
        heap_extract_min(q);
        if (cur_d > d[v]) {
            continue;
        }
        for (int j = 0; j < links[v].size; j++) {
            int to = links[v].arr[j].to;
            int weight = links[v].arr[j].weight;
            if (d[v] + weight < d[to]) {
                d[to] = d[v] + weight;
                p[to] = v;
                heap_add_element(q, (HeapNode) {.weight = d[to], .num = to});
            }
        }
    }
    int ret = d[to];
    path->size = 0;
    for (int i = to; i != from; i = p[i]) {
        path->nodes[path->size] = i;
        path->size++;
    }
    path->nodes[path->size] = from;
    path->size++;
    APP_LOG(APP_LOG_LEVEL_INFO, "path size: %d", path->size);
    heap_destroy(q);
    free(d);
    free(p);
    return ret;
}
