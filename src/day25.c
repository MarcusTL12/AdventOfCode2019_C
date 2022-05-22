#include <stdio.h>
#include <stdlib.h>

#include "aoc_util/vector.h"
#include "aoc_util/file_util.h"
#include "intcode.h"

void d25p1() { }

void d25p2() {
    intcode_machine machine = intcode_from_file("input/day25/input");
    vec_t sbuf = vec_create(sizeof(char));

    intcode_run(&machine);
    for (int64_t buf; intcode_recieve(&machine, &buf);) vec_push(&sbuf, &buf);
    printf("%.*s\n", (int)sbuf.len, (char *)sbuf.data);

    char inp[1024];

    while (!machine.halted) {
        scanf("%1023[^\n]", inp);
        flushstdin();
        for (char *c = inp; *c; c++) {
            intcode_send(&machine, *c);
        }
        intcode_send(&machine, 10);
        intcode_run(&machine);
        vec_clear(&sbuf);
        for (int64_t buf; intcode_recieve(&machine, &buf);) {
            vec_push(&sbuf, &buf);
        }
        printf("%.*s\n", (int)sbuf.len, (char *)sbuf.data);
    }

    intcode_free(&machine);
}
