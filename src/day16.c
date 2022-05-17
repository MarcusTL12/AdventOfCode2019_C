#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "aoc_util/file_util.h"

void d16p1() {
    char *inp = file_read_full("input/day16/input");

    size_t len = strlen(inp);
    while (!isdigit(inp[len - 1])) len--;

    int *buf = malloc(sizeof(int) * len * 2);
    int *s1 = buf, *s2 = buf + len;

    for (size_t i = 0; i < len; i++) {
        s1[i] = inp[i] - '0';
    }

    const int pattern[] = {0, 1, 0, -1};

    for (size_t k = 0; k < 100; k++) {
        for (size_t i = 0; i < len; i++) {
            int n = 0;
            for (size_t j = 0; j < len; j++) {
                n += s1[j] * pattern[((j + 1) / (i + 1)) % 4];
            }
            s2[i] = (n > 0 ? n : -n) % 10;
        }
        int *tmp = s1;
        s1 = s2;
        s2 = tmp;
    }

    char ans[8];

    for (size_t i = 0; i < 8; i++) {
        ans[i] = '0' + s1[i];
    }

    printf("%.*s\n", 8, ans);

    free(buf);
    free(inp);
}

void d16p2() {
    char *inp = file_read_full("input/day16/input");

    size_t index;

    {
        char tmp = inp[7];
        inp[7] = 0;
        index = atoll(inp);
        inp[7] = tmp;
    }

    size_t len = strlen(inp);
    while (!isdigit(inp[len - 1])) len--;

    int *signal = malloc(sizeof(int) * len);

    for (size_t i = 0; i < len; i++) {
        signal[i] = inp[i] - '0';
    }

    size_t actual_len = 10000 * len - index;
    int *actual_signal = malloc(sizeof(int) * actual_len);

    for (size_t i = index; i < 10000 * len; i++) {
        size_t j = i - index;
        actual_signal[j] = signal[i % len];
    }

    for (size_t j = 0; j < 100; j++) {
        for (int64_t i = actual_len - 2; i >= 0; i--) {
            actual_signal[i] += actual_signal[i + 1];
            actual_signal[i] %= 10;
        }
    }

    char ans[8];

    for (size_t i = 0; i < 8; i++) {
        ans[i] = '0' + actual_signal[i];
    }

    printf("%.*s\n", 8, ans);

    free(actual_signal);
    free(signal);
    free(inp);
}