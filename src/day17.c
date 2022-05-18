#include <stdio.h>
#include <stdlib.h>

#include "intcode.h"

void d17p1() {
    intcode_machine machine = intcode_from_file("input/day17/input");

    intcode_run(&machine);

    size_t len = intcode_out_len(&machine), stride = 0;
    char *s = malloc(len);
    for (int64_t i = 0, j; intcode_recieve(&machine, &j); i++) {
        s[i] = j;
        if (!stride && j == '\n') {
            stride = i + 1;
        }
    }

    size_t w = stride - 1, h = len / stride;

    size_t intersects = 0;

    for (size_t i = 1; i < h - 1; i++) {
        for (size_t j = 1; j < w - 1; j++) {
            if (s[i * stride + j] == '#' && s[(i - 1) * stride + j] == '#' &&
                s[(i + 1) * stride + j] == '#' &&
                s[i * stride + j - 1] == '#' && s[i * stride + j + 1] == '#') {
                intersects += i * j;
            }
        }
    }

    printf("%zu\n", intersects);

    free(s);
    intcode_free(&machine);
}

void d17p2() {
    intcode_machine machine = intcode_from_file("input/day17/input");

    *intcode_getmem(&machine, 0) = 2;

    for (char *ins =
             "A,B,A,B,C,C,B,A,B,C\n"
             "L,4,R,8,L,6,L,10\n"
             "L,6,R,8,R,10,L,6,L,6\n"
             "L,4,L,4,L,10\n"
             "n\n";
         *ins; ins++) {
        intcode_send(&machine, *ins);
    }
    intcode_run(&machine);

    int64_t ans;
    while (intcode_recieve(&machine, &ans));

    printf("%li\n", ans);

    intcode_free(&machine);
}
