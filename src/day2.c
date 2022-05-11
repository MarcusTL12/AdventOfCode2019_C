#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_util/file_util.h"
#include "aoc_util/show.h"
#include "aoc_util/vector.h"

static void run_code(int *prog) {
    bool halted = false;
    size_t ip = 0;

    while (!halted) {
        switch (prog[ip]) {
            case 1:
                prog[prog[ip + 3]] = prog[prog[ip + 1]] + prog[prog[ip + 2]];
                ip += 4;
                break;
            case 2:
                prog[prog[ip + 3]] = prog[prog[ip + 1]] * prog[prog[ip + 2]];
                ip += 4;
                break;

            case 99:
                halted = true;
                break;

            default:
                printf("Invalid Instruction: %i\n", prog[ip]);
                halted = true;
                break;
        }
    }
}

void d2p1() {
    char *inp = file_read_full("input/day2/input");

    vec_t v = vec_create(sizeof(int));

    for (char *c = inp; *c;) {
        int i = atoi(c);
        vec_push(&v, &i);
        while (isdigit(*c)) c++;
        while (!isdigit(*c) && *c) c++;
    }

    int *prog = v.data;

    prog[1] = 12;
    prog[2] = 2;

    run_code(prog);

    printf("prog[0] = %i\n", prog[0]);

    vec_free(&v);
    free(inp);
}

void d2p2() {
    char *inp = file_read_full("input/day2/input");

    vec_t v = vec_create(sizeof(int));

    for (char *c = inp; *c;) {
        int i = atoi(c);
        vec_push(&v, &i);
        while (isdigit(*c)) c++;
        while (!isdigit(*c) && *c) c++;
    }

    int *orig_prog = v.data;
    int *prog = malloc(v.len * sizeof(int));

    int ans = 0;

    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            memcpy(prog, orig_prog, v.len * sizeof(int));
            prog[1] = noun;
            prog[2] = verb;
            run_code(prog);
            if (prog[0] == 19690720) {
                ans = 100 * noun + verb;
                break;
            }
        }

        if (ans) break;
    }

    printf("100 * noun + verb = %i\n", ans);

    free(prog);
    vec_free(&v);
    free(inp);
}
