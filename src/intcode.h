#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "aoc_util/deque.h"
#include "aoc_util/vector.h"

typedef struct {
    vec_t program;
    deque_t inp_queue, out_queue;
    size_t pc;
    int64_t relbase;
    bool halted;
} intcode_machine;

intcode_machine intcode_from_string(char *s);
intcode_machine intcode_from_file(char *filename);
void intcode_free(intcode_machine *machine);

int64_t *intcode_getmem(intcode_machine *machine, size_t i);

void intcode_send(intcode_machine *machine, int64_t v);
bool intcode_recieve(intcode_machine *machine, int64_t *dest);

// amt = -1 gives "infinite"
size_t intcode_recieve_multiple(intcode_machine *machine, int64_t *dest,
                                size_t amt);

void intcode_run(intcode_machine *machine);

intcode_machine intcode_clone(intcode_machine *machine);
void intcode_reset(intcode_machine *machine, vec_t *program);
