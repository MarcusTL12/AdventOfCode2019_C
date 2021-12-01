#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../AoC_C_utils/src/bitfield.h"
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

static struct hashmap *paint(bool init_color) {
    intcode_machine machine = intcode_from_file("input/day11/input");

    struct hashmap *map =
        hashmap_new(sizeof(tile_t), 0, 0, 0, point_hash, point_cmp, NULL, NULL);

    tile_t pos = {0, 0, init_color};
    int64_t dir[2] = {0, -1};

    hashmap_set(map, &pos);

    while (true) {
        intcode_send(&machine, pos.color);
        intcode_run(&machine);
        int64_t color, rot;
        if (!intcode_recieve(&machine, &color) ||
            !intcode_recieve(&machine, &rot)) {
            break;
        }

        pos.color = color;

        hashmap_set(map, &pos);

        int64_t fac = (rot << 1) - 1, tmp = dir[0];
        dir[0] = -dir[1] * fac;
        dir[1] = tmp * fac;

        pos.x += dir[0];
        pos.y += dir[1];

        tile_t *buf = hashmap_get(map, &pos);
        if (buf) {
            pos.color = buf->color;
        } else {
            pos.color = false;
        }
    }

    intcode_free(&machine);

    return map;
}

void d11p1() {
    struct hashmap *map = paint(false);

    printf("%zu\n", hashmap_count(map));

    hashmap_free(map);
}

static bool find_minmax(const void *p_, void *udata) {
    const int64_t *p = p_;

    int64_t *minmax = udata;

    minmax[0] = p[0] < minmax[0] ? p[0] : minmax[0];
    minmax[1] = p[1] < minmax[1] ? p[1] : minmax[1];
    minmax[2] = p[0] > minmax[2] ? p[0] : minmax[2];
    minmax[3] = p[1] > minmax[3] ? p[1] : minmax[3];

    return true;
}

static bool fill_canvas(const void *p_, void *udata_) {
    const tile_t *p = p_;

    void **udata = udata_;
    size_t *width = udata[0];
    int64_t *x_off = udata[1], *y_off = udata[2];
    uint64_t *canvas = udata[3];

    int64_t x = p->x + *x_off, y = p->y + *y_off;

    bitfield_set(canvas, x + y * *width, p->color);

    return true;
}

void d11p2() {
    struct hashmap *map = paint(true);

    int64_t minmax[4] = {INT64_MAX, INT64_MAX, INT64_MIN, INT64_MIN};

    hashmap_scan(map, find_minmax, minmax);

    size_t width = minmax[2] - minmax[0] + 1,
           height = minmax[3] - minmax[1] + 1;

    int64_t x_off = -minmax[0], y_off = -minmax[1];

    uint64_t *canvas = bitfield_create(width * height);

    void *udata[4] = {&width, &x_off, &y_off, canvas};

    hashmap_scan(map, fill_canvas, udata);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            printf("%s", bitfield_get(canvas, i * width + j) ? "██" : "  ");
        }
        printf("\n");
    }

    bitfield_free(canvas);
    hashmap_free(map);
}
