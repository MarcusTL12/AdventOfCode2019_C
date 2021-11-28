#include <stdio.h>
#include <stdlib.h>

#include "../AoC_C_utils/src/deque.h"
#include "../AoC_C_utils/src/show.h"
#include "intcode.h"

void d5p1() {
    intcode_machine machine = intcode_from_file("input/day5/input");

    intcode_send(&machine, 1);

    intcode_run(&machine);

    int64_t x;

    while (intcode_recieve(&machine, &x)) {
        printf("%ld\n", x);
    }

    intcode_free(&machine);
}

void d5p2() {}
