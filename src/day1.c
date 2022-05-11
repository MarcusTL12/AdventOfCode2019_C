#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "aoc_util/file_util.h"

static int fuel(int mod_mass) { return mod_mass / 3 - 2; }

void d1p1() {
    char *inp = file_read_full("input/day1/input");

    int total = 0;

    for (char *c = inp; *c;) {
        int mod_mass = atoi(c);

        total += fuel(mod_mass);

        while (isdigit(*c)) c++;
        while (!isdigit(*c) && *c) c++;
    }

    printf("Total fuel: %i\n", total);

    free(inp);
}

static int rec_fuel(int mass) {
    int f = fuel(mass);
    if (f > 0) {
        return f + rec_fuel(f);
    } else {
        return 0;
    }
}

void d1p2() {
    char *inp = file_read_full("input/day1/input");

    int total = 0;

    for (char *c = inp; *c;) {
        int mod_mass = atoi(c);

        total += rec_fuel(mod_mass);

        while (isdigit(*c)) c++;
        while (!isdigit(*c) && *c) c++;
    }

    printf("Total fuel: %i\n", total);

    free(inp);
}
