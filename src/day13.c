#include <stdio.h>

#include "intcode.h"

void d13p1() {
    intcode_machine machine = intcode_from_file("input/day13/input");

    intcode_run(&machine);

    size_t ans = 0;

    for (int64_t buf[3]; intcode_recieve_multiple(&machine, buf, 3);) {
        ans += buf[2] == 2 ? 1 : 0;
    }

    printf("%zu\n", ans);

    intcode_free(&machine);
}

char *get_board(intcode_machine *machine) {
    intcode_run(machine);

    vec_t commands = vec_create(sizeof(int64_t) * 3);

    size_t width = 0, height = 0;

    for (int64_t buf[3]; intcode_recieve_multiple(&machine, buf, 3);) {
        vec_push(&commands, buf);
        width = buf[0] > width ? buf[0] : width;
        height = buf[1] > height ? buf[1] : height;
    }

    char *board = malloc(width * height);

    for (size_t i = 0; i < commands.len; i++) {
    }

    vec_free(&commands);

    return board;
}

void d13p2() {
    intcode_machine machine = intcode_from_file("input/day13/input");

    ((int64_t *)(machine.program.data))[0] = 2;

    get_board(&machine);

    intcode_free(&machine);
}
