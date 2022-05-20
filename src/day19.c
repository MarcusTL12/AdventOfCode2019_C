#include <stdio.h>

#include "aoc_util/point.h"
#include "intcode.h"

static bool pulled(intcode_machine *machine, vec_t *program, int64_t x,
                   int64_t y) {
    intcode_reset(machine, program);

    intcode_send(machine, x);
    intcode_send(machine, y);
    intcode_run(machine);

    int64_t ans;
    intcode_recieve(machine, &ans);

    return ans;
}

void d19p1() {
    intcode_machine machine = intcode_from_file("input/day19/input");
    vec_t program = vec_clone(&machine.program);

    int64_t ans = 0;

    for (int64_t y = 0; y < 50; y++) {
        for (int64_t x = 0; x < 50; x++) {
            ans += pulled(&machine, &program, x, y);
        }
    }

    printf("%li\n", ans);

    vec_free(&program);
    intcode_free(&machine);
}

static point2i widestat(intcode_machine *machine, vec_t *program, int64_t y) {
    int64_t x = 0;
    while (!pulled(machine, program, x, y)) x++;

    int64_t i = 0;
    while (pulled(machine, program, x + i, y - i)) i++;

    return (point2i){i, x};
}

void d19p2() {
    intcode_machine machine = intcode_from_file("input/day19/input");
    vec_t program = vec_clone(&machine.program);

    int64_t lo = 8, hi = 16;

    while (widestat(&machine, &program, hi).x < 100) {
        lo = hi;
        hi *= 2;
    }

    while (lo < hi) {
        int64_t mid = (lo + hi) / 2;
        if (widestat(&machine, &program, mid).x < 100) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    int64_t x = widestat(&machine, &program, lo).y;

    printf("%li\n", x * 10000 + lo - 99);

    vec_free(&program);
    intcode_free(&machine);
}
