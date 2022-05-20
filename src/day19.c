#include <stdio.h>

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

void d19p2() {}