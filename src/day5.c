#include <stdio.h>
#include <stdlib.h>

#include "aoc_util/deque.h"
#include "aoc_util/show.h"
#include "intcode.h"

void d5p1() {
    intcode_machine machine = intcode_from_file("input/day5/input");

    intcode_send(&machine, 1);

    intcode_run(&machine);

    int64_t x;

    while (intcode_recieve(&machine, &x))
        ;

    printf("%ld\n", x);

    intcode_free(&machine);
}

void d5p2() {
    intcode_machine machine = intcode_from_file("input/day5/input");

    intcode_send(&machine, 5);

    intcode_run(&machine);

    int64_t x;

    while (intcode_recieve(&machine, &x)) {
        printf("%ld\n", x);
    }

    intcode_free(&machine);
}
