#include <stdio.h>

#include "intcode.h"

static bool pulled(intcode_machine *machine, int64_t x, int64_t y) {
    intcode_machine clone = intcode_clone(machine);

    intcode_send(&clone, x);
    intcode_send(&clone, y);
    intcode_run(&clone);

    int64_t ans;
    intcode_recieve(&clone, &ans);

    intcode_free(&clone);

    return ans;
}

void d19p1() {
    intcode_machine machine = intcode_from_file("input/day19/input");

    int64_t ans = 0;

    for (int64_t y = 0; y < 50; y++) {
        for (int64_t x = 0; x < 50; x++) {
            ans += pulled(&machine, x, y);
        }
    }

    printf("%li\n", ans);

    intcode_free(&machine);
}

void d19p2() {

}