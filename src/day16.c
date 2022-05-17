#include <ctype.h>
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

void d16p2() {}