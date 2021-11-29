#include <stdio.h>
#include <string.h>

#include "../AoC_C_utils/src/combinatorics.h"
#include "../AoC_C_utils/src/show.h"
#include "../AoC_C_utils/src/vector.h"
#include "intcode.h"

const size_t N = 5;

static int64_t single_pass(intcode_machine *machines, size_t *seq) {
    for (size_t i = 0; i < N; i++) {
        intcode_send(&machines[i], seq[i]);
    }

    intcode_send(machines, 0);
    intcode_run(machines);

    int64_t buf;
    for (size_t i = 1; i < N; i++) {
        intcode_recieve(&machines[i - 1], &buf);
        intcode_send(&machines[i], buf);
        intcode_run(&machines[i]);
    }

    intcode_recieve(&machines[N - 1], &buf);
    return buf;
}

static void reset_machines(intcode_machine *machines, vec_t *program) {
    for (size_t i = 0; i < N; i++) {
        intcode_reset(&machines[i], program);
    }
}

void d7p1() {
    intcode_machine orig_machine = intcode_from_file("input/day7/input");

    intcode_machine machines[N];
    for (int i = 0; i < N; i++) {
        machines[i] = intcode_clone(&orig_machine);
    }

    size_t permstate[N], perm[N];
    memset(permstate, 0, N * sizeof(size_t));

    int64_t ans = 0;

    do {
        get_perm(perm, permstate, N);

        reset_machines(machines, &orig_machine.program);

        int64_t score = single_pass(machines, perm);

        ans = score > ans ? score : ans;
    } while (next_permstate(permstate, N));

    printf("Max: %ld\n", ans);

    for (int i = 0; i < N; i++) {
        intcode_free(&machines[i]);
    }
    intcode_free(&orig_machine);
}

static int64_t multi_pass(intcode_machine *machines, size_t *seq) {
    for (size_t i = 0; i < N; i++) {
        intcode_send(&machines[i], seq[i] + 5);
    }

    bool running = true;

    int64_t buf = 0, ans = 0;
    while (running) {
        intcode_send(machines, buf);
        intcode_run(machines);
        for (size_t i = 1; i < N; i++) {
            running = intcode_recieve(&machines[i - 1], &buf);
            if (!running) break;
            intcode_send(&machines[i], buf);
            intcode_run(&machines[i]);
        }
        if (!running) break;
        running = intcode_recieve(&machines[N - 1], &buf);
        if (running) ans = buf;
    }

    return ans;
}

void d7p2() {
    intcode_machine orig_machine = intcode_from_file("input/day7/input");

    intcode_machine machines[N];
    for (int i = 0; i < N; i++) {
        machines[i] = intcode_clone(&orig_machine);
    }

    size_t permstate[N], perm[N];
    memset(permstate, 0, N * sizeof(size_t));

    int64_t ans = 0;

    do {
        get_perm(perm, permstate, N);

        reset_machines(machines, &orig_machine.program);

        int64_t score = multi_pass(machines, perm);

        ans = score > ans ? score : ans;
    } while (next_permstate(permstate, N));

    printf("Max: %ld\n", ans);

    for (int i = 0; i < N; i++) {
        intcode_free(&machines[i]);
    }
    intcode_free(&orig_machine);
}
