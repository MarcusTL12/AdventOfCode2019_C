#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "aoc_util/bitfield.h"
#include "aoc_util/file_util.h"
#include "aoc_util/point.h"

static void load_maze(char *filename, char **maze, size_t *w, size_t *h,
                      point2i *keys, point2i *doors, size_t *amt_keys,
                      point2i *start) {
    *maze = file_read_full("input/day18/ex1");

    *w = 0;
    for (char *c = maze; *c != '\n'; c++) (*w)++;
    *h = (strlen(maze) + 1) / (*w + 1);

    *amt_keys = 0;

    for (size_t y = 0; y < *h; y++) {
        for (size_t x = 0; x < *w; x++) {
            char c = (*maze)[y * (*w + 1) + x];

            if (isalpha(c)) {
                point2i *v = islower(c) ? keys : doors;
                size_t ind = c & 0b11111;
                v[ind - 1] = (point2i){x, y};
                if (islower(c)) {
                    *amt_keys = ind > *amt_keys ? ind : *amt_keys;
                }
            } else if (c == '@') {
                *start = (point2i){x, y};
                (*maze)[y * (*w + 1) + x] = '.';
            }
        }
    }
}

void d18p1() {
    char *maze;
    size_t w, h, amt_keys;
    point2i keys[26], doors[26], start;
    load_maze("input/day18/ex1", &maze, &w, &h, keys, doors, &amt_keys, &start);

    printf("%zu, %zu, %zu\n", w, h, amt_keys);



    free(maze);
}

void d18p2() {}