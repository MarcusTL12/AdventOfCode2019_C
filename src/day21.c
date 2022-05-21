#include <stdio.h>

#include "intcode.h"

void d21p1() {
    intcode_machine machine = intcode_from_file("input/day21/input");

    for (char *ins =
             "NOT T T\n"
             "AND A T\n"
             "AND B T\n"
             "AND C T\n"
             "NOT T J\n"
             "AND D J\n"
             "WALK\n";
         *ins; ins++) {
        intcode_send(&machine, *ins);
    }

    intcode_run(&machine);

    int64_t ans;
    while (intcode_recieve(&machine, &ans))
        ;

    printf("%li\n", ans);

    intcode_free(&machine);
}

void d21p2() {
    intcode_machine machine = intcode_from_file("input/day21/input");

    for (char *ins =
             "NOT T T\n"
             "AND A T\n"
             "AND B T\n"
             "AND C T\n"
             "NOT T J\n"
             "AND D J\n"
             "NOT J T\n"
             "NOT T T\n"
             "AND E T\n"
             "OR H T\n"
             "AND T J\n"
             "RUN\n";
         *ins; ins++) {
        intcode_send(&machine, *ins);
    }

    intcode_run(&machine);

    int64_t ans;
    while (intcode_recieve(&machine, &ans))
        ;

    printf("%li\n", ans);

    intcode_free(&machine);
}
