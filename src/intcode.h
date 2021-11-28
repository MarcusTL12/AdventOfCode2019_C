#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../AoC_C_utils/src/deque.h"
#include "../AoC_C_utils/src/vector.h"

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

void intcode_run(intcode_machine *machine);