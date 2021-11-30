#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/bitfield.h"
#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/number_theory.h"

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

static size_t amt_visible(char *map, size_t width, size_t height, uint64_t *set,
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

    return bitfield_count(set, set_ints);
}

static void findmax(char *map, size_t width, size_t height, size_t *x_,
                    size_t *y_, size_t *max) {
    uint64_t *set = malloc((1 + (width * height - 1) / 64) * sizeof(uint64_t));

    *max = 0;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (map[x + width * y] == '#') {
                size_t amt = amt_visible(map, width, height, set, x, y);
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

void d10p2() {}
