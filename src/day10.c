#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_util/bitfield.h"
#include "aoc_util/file_util.h"
#include "aoc_util/number_theory.h"
#include "aoc_util/vector.h"

static char *parse_input(char *filename, size_t *width, size_t *height) {
    char *inp = file_read_full(filename);

    *width = 0;
    *height = 0;

    while (inp[*width] != '\n') (*width)++;
    while (inp[*height * (*width + 1)] && inp[*height * (*width + 1) + 1]) {
        (*height)++;
    }

    char *map = malloc(*width * *height);

    for (size_t i = 0; i < *height; i++) {
        memcpy(map + i * *width, inp + i * (*width + 1), *width);
    }

    return map;
}

static void print_map(char *map, size_t width, size_t height) {
    for (size_t i = 0; i < height; i++) {
        printf("%.*s\n", (int)width, map + width * i);
    }
}

static void fill_visible(char *map, size_t width, size_t height, uint64_t *set,
                         size_t x, size_t y) {
    size_t set_ints = 1 + (width * height - 1) / 64;
    memset(set, 0, set_ints * sizeof(uint64_t));

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (map[i * width + j] == '#' && (i != y || j != x)) {
                int64_t a = i - y, b = j - x;

                int64_t g = gcd(a, b);
                a /= g;
                b /= g;
                a += y;
                b += x;

                bitfield_set(set, a * width + b, true);
            }
        }
    }
}

static void findmax(char *map, size_t width, size_t height, size_t *x_,
                    size_t *y_, size_t *max) {
    size_t set_ints = 1 + (width * height - 1) / 64;
    uint64_t *set = malloc(set_ints * sizeof(uint64_t));

    *max = 0;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (map[x + width * y] == '#') {
                fill_visible(map, width, height, set, x, y);
                size_t amt = bitfield_count(set, set_ints);
                if (amt > *max) {
                    *max = amt;
                    *x_ = x;
                    *y_ = y;
                }
            }
        }
    }

    free(set);
}

void d10p1() {
    size_t width, height;
    char *map = parse_input("input/day10/input", &width, &height);

    size_t x, y, max;

    findmax(map, width, height, &x, &y, &max);

    printf("%zu\n", max);

    free(map);
}

static void print_visible(uint64_t *set, size_t width, size_t height) {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            printf("%c", bitfield_get(set, i * width + j) ? '#' : '.');
        }
        printf("\n");
    }
}

static void find_visible(char *map, size_t width, size_t height, size_t x,
                         size_t y, uint64_t *set, vec_t *target) {
    fill_visible(map, width, height, set, x, y);

    vec_clear(target);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            if (bitfield_get(set, i * width + j)) {
                int64_t p[2] = {j - x, i - y};

                vec_push(target, &p);
            }
        }
    }
}

static void print_point(void *point) {
    printf("(%ld, %ld)", ((int64_t *)point)[0], ((int64_t *)point)[1]);
}

static int quadrant(int64_t *p) {
    if (p[0] >= 0 && p[1] < 0) {
        return 0;
    } else if (p[0] > 0 && p[1] >= 0) {
        return 1;
    } else if (p[0] <= 0 && p[1] > 0) {
        return 2;
    } else {
        return 3;
    }
}

static int cmp_points(const void *p1_, const void *p2_) {
    int64_t *p1 = (int64_t *)p1_, *p2 = (int64_t *)p2_;

    int p1_quad = quadrant(p1), p2_quad = quadrant(p2);

    if (p1_quad - p2_quad) {
        return p1_quad - p2_quad;
    } else {
        return p1[1] * p2[0] - p1[0] * p2[1];
    }
}

void d10p2() {
    size_t width, height;
    char *map = parse_input("input/day10/input", &width, &height);

    size_t x, y, max;

    findmax(map, width, height, &x, &y, &max);

    vec_t vis_buf = vec_create(2 * sizeof(int64_t));
    size_t set_ints = 1 + (width * height - 1) / 64;
    uint64_t *set = malloc(set_ints * sizeof(uint64_t));

    find_visible(map, width, height, x, y, set, &vis_buf);

    qsort(vis_buf.data, vis_buf.len, 2 * sizeof(int64_t), cmp_points);

    size_t i = 0;

    bool done = false;

    do {
        find_visible(map, width, height, x, y, set, &vis_buf);
        qsort(vis_buf.data, vis_buf.len, 2 * sizeof(int64_t), cmp_points);
        for (size_t j = 0; j < vis_buf.len; j++) {
            int64_t *dir = vec_get(&vis_buf, j);
            int64_t cur_x = x, cur_y = y;

            do {
                cur_x += dir[0];
                cur_y += dir[1];
            } while (map[cur_x + width * cur_y] == '.');

            map[cur_x + width * cur_y] = '.';

            if (++i == 200) {
                printf("%ld\n", cur_x * 100 + cur_y);
                done = true;
                break;
            }
        }
    } while (vis_buf.len && !done);

    vec_free(&vis_buf);
    free(map);
}
