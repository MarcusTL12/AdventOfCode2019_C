#include "intcode.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "../AoC_C_utils/src/file_util.h"

intcode_machine intcode_from_string(char *s) {
    intcode_machine machine;

    machine.program = vec_create(sizeof(int64_t));
    machine.inp_queue = deque_create(sizeof(int64_t));
    machine.out_queue = deque_create(sizeof(int64_t));
    machine.pc = 0;
    machine.relbase = 0;
    machine.halted = false;

    while (*s) {
        int64_t i = atoll(s);
        vec_push(&machine.program, &i);
        while (isdigit(*s) || *s == '-') s++;
        while (!isdigit(*s) && *s != '-' && *s) s++;
    }

    return machine;
}

intcode_machine intcode_from_file(char *filename) {
    char *s = file_read_full(filename);
    intcode_machine machine = intcode_from_string(s);

    free(s);
    return machine;
}

void intcode_free(intcode_machine *machine) {
    vec_free(&machine->program);
    deque_free(&machine->inp_queue);
    deque_free(&machine->out_queue);
}

intcode_machine intcode_clone(intcode_machine *machine) {
    intcode_machine new_machine;

    new_machine.program = vec_clone(&machine->program);
    new_machine.inp_queue = deque_clone(&machine->inp_queue);
    new_machine.out_queue = deque_clone(&machine->out_queue);
    new_machine.pc = machine->pc;
    new_machine.relbase = machine->relbase;
    new_machine.halted = machine->halted;

    return new_machine;
}

void intcode_reset(intcode_machine *machine, vec_t *program) {
    vec_copy(&machine->program, program);
    deque_clear(&machine->inp_queue);
    deque_clear(&machine->out_queue);
    machine->pc = 0;
    machine->relbase = 0;
    machine->halted = false;
}

int64_t *intcode_getmem(intcode_machine *machine, size_t i) {
    if (i >= machine->program.len) {
        for (size_t j = machine->program.len; j < i; j++) {
            int64_t zero = 0;
            vec_push(&machine->program, &zero);
        }
    }
    return vec_get(&machine->program, i);
}

void intcode_send(intcode_machine *machine, int64_t v) {
    deque_push_back(&machine->inp_queue, &v);
}

bool intcode_recieve(intcode_machine *machine, int64_t *dest) {
    return deque_pop_front(&machine->out_queue, dest);
}

static int64_t pow10(size_t e) {
    int64_t ans = 1;
    while (e) {
        ans *= 10;
        e--;
    }
    return ans;
}

static int64_t getmode(intcode_machine *machine, size_t i) {
    return ((*intcode_getmem(machine, machine->pc) / pow10(i + 1)) % 10);
}

static int64_t getval(intcode_machine *machine, size_t i) {
    int64_t val = *intcode_getmem(machine, machine->pc + i);

    switch (getmode(machine, i)) {
        case 0:
            return *intcode_getmem(machine, val);
        case 1:
            return val;
        case 2:
            return *intcode_getmem(machine, val + machine->relbase);

        default:
            printf("Invalid memory mode for reading: %ld\n",
                   getmode(machine, i));
            exit(0);
            return 0;
    }
}

static void setval(intcode_machine *machine, size_t i, int64_t v) {
    int64_t val = *intcode_getmem(machine, machine->pc + i);

    switch (getmode(machine, i)) {
        case 0:
            *intcode_getmem(machine, val) = v;
            break;
        case 2:
            *intcode_getmem(machine, val + machine->relbase) = v;
            break;

        default:
            printf("Invalid memory mode for writing: %ld\n",
                   getmode(machine, i));
            exit(0);
    }
}

void intcode_run(intcode_machine *machine) {
    while (true) {
        int64_t instruction = *intcode_getmem(machine, machine->pc);

        int64_t x;

        switch (instruction % 100) {
            case 1:
                setval(machine, 3, getval(machine, 1) + getval(machine, 2));
                machine->pc += 4;
                break;
            case 2:
                setval(machine, 3, getval(machine, 1) * getval(machine, 2));
                machine->pc += 4;
                break;
            case 3:
                if (deque_pop_front(&machine->inp_queue, &x)) {
                    setval(machine, 1, x);
                    machine->pc += 2;
                } else {
                    return;
                }
                break;
            case 4:
                x = getval(machine, 1);
                deque_push_back(&machine->out_queue, &x);
                machine->pc += 2;
                break;
            case 5:
                if (getval(machine, 1) != 0) {
                    machine->pc = getval(machine, 2);
                } else {
                    machine->pc += 3;
                }
                break;
            case 6:
                if (getval(machine, 1) == 0) {
                    machine->pc = getval(machine, 2);
                } else {
                    machine->pc += 3;
                }
                break;
            case 7:
                setval(machine, 3, getval(machine, 1) < getval(machine, 2));
                machine->pc += 4;
                break;
            case 8:
                setval(machine, 3, getval(machine, 1) == getval(machine, 2));
                machine->pc += 4;
                break;
            case 9:
                machine->relbase += getval(machine, 1);
                machine->pc += 2;
                break;
            case 99:
                machine->halted = true;
                return;

            default:
                printf("Invalid instruction: %ld\n", instruction);
                exit(0);
        }
    }
}
