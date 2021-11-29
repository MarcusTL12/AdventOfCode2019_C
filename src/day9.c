#include <stdio.h>

#include "../AoC_C_utils/src/show.h"
#include "intcode.h"

void d9p1() {
    intcode_machine machine = intcode_from_file("input/day9/ex1");

    vec_show(&machine.program, show_int64);
    printf("\n");

    // intcode_send(&machine, 2);

    intcode_run(&machine);

    int64_t buf;

    while (intcode_recieve(&machine, &buf)) {
        printf("%ld\n", buf);
    }

    intcode_free(&machine);
}

void d9p2() {}
