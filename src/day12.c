#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/regex_util.h"
#include "../AoC_C_utils/src/show.h"
#include "../AoC_C_utils/src/vector.h"

// takes in only one axis
static void do_step(int64_t *coors, int64_t *velocities, size_t moons) {
    for (size_t i = 0; i < moons; i++) {
        for (size_t j = 0; j < moons; j++) {
            int64_t diff = coors[j] - coors[i];
            int64_t delta = diff > 0 ? 1 : diff ? 0 : -1;
        }
    }
}

void d12p1() {
    char *inp = file_read_full("input/day12/ex1");
    void *reg = regex_compile("<x=(-?\\d+), y=(-?\\d+), z=(-?\\d+)>");

    size_t amt_moons = 0;
    int64_t *coords;

    {
        vec_t coords_vec = vec_create(sizeof(int64_t));
        regex_iter it = regex_iter_create(reg, inp);

        for (size_t captures[8]; regex_next_match(&it, captures);) {
            int64_t x = atoll(inp + captures[2]), y = atoll(inp + captures[4]),
                    z = atoll(inp + captures[6]);

            for (size_t i = 1; i <= 3; i++) {
                int64_t r = atoll(inp + captures[2 * i]);
                vec_push(&coords_vec, &r);
            }

            amt_moons++;
        }

        regex_iter_free(it);

        coords = malloc(sizeof(int64_t) * amt_moons);

        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < amt_moons; j++) {
                coords[i * amt_moons + j] =
                    ((int64_t *)coords_vec.data)[j * amt_moons + j];
            }
        }

        vec_free(&coords_vec);
    }

    int64_t *velocities = malloc(sizeof(int64_t) * amt_moons);
    memset(velocities, 0, sizeof(int64_t) * amt_moons);

    free(velocities);
    regex_free(reg);
    free(inp);
}

void d12p2() {}
