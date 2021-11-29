#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"

const size_t WIDTH = 25, HEIGHT = 6;

void d8p1() {
    char *inp = file_read_full("input/day8/input");

    int64_t ans = 0, min_zeros = INT64_MAX;

    for (char *c = inp; isdigit(*c);) {
        int64_t zeros = 0, ones = 0, twos = 0;
        for (size_t i = 0; i < WIDTH * HEIGHT; i++, c++) {
            switch (*c) {
                case '0':
                    zeros++;
                    break;
                case '1':
                    ones++;
                    break;
                case '2':
                    twos++;
            }
        }

        if (zeros < min_zeros) {
            min_zeros = zeros;
            ans = ones * twos;
        }
    }

    printf("%ld\n", ans);

    free(inp);
}

void d8p2() {
    char *inp = file_read_full("input/day8/input");

    char buffer[WIDTH * HEIGHT];

    memset(buffer, '2', WIDTH * HEIGHT);

    for (char *c = inp; isdigit(*c); c += WIDTH * HEIGHT) {
        for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
            buffer[i] = buffer[i] == '2' ? c[i] : buffer[i];
        }
    }

    for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
        buffer[i] = buffer[i] == '1' ? '#' : ' ';
    }

    for (size_t y = 0; y < HEIGHT; y++) {
        printf("%.*s\n", (int)WIDTH, &buffer[y * WIDTH]);
    }

    free(inp);
}
