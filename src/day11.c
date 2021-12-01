#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../hashmap.c/hashmap.h"
#include "intcode.h"

static uint64_t point_hash(const void *p, uint64_t s0, uint64_t s1) {
    return hashmap_sip(p, 2 * sizeof(int64_t), s0, s1);
}

static int point_cmp(const void *a, const void *b, void *udata) {
    return memcmp(a, b, 2 * sizeof(int64_t));
}

typedef struct {
    int64_t x, y;
    bool color;
} tile_t;

void d11p1() {
    intcode_machine machine = intcode_from_file("input/day11/input");
    vec_t path = vec_create(2 * sizeof(int64_t));

    struct hashmap *map =
        hashmap_new(sizeof(tile_t), 0, 0, 0, point_hash, point_cmp, NULL, NULL);

    tile_t pos = {0, 0, false};
    int64_t dir[2] = {0, 1};

    hashmap_set(map, &pos);

    while (true) {
        intcode_send(&machine, pos.color);
        intcode_run(&machine);
        int64_t color, dir;
        if (!intcode_recieve(&machine, &color) ||
            !intcode_recieve(&machine, &dir)) {
            break;
        }

        pos.color = color;
    }

    hashmap_free(map);
    vec_free(&path);
    intcode_free(&machine);
}

void d11p2() {}
