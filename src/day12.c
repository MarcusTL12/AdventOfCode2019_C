#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc_util/file_util.h"
#include "aoc_util/number_theory.h"
#include "aoc_util/regex_util.h"
#include "aoc_util/show.h"
#include "aoc_util/vector.h"

// takes in only one axis
static void do_step(int64_t *coords, int64_t *velocities, size_t moons) {
    for (size_t i = 0; i < moons; i++) {
        for (size_t j = i + 1; j < moons; j++) {
            int64_t diff = coords[j] - coords[i];
            int64_t delta = diff > 0 ? 1 : diff ? -1 : 0;
            velocities[i] += delta;
            velocities[j] -= delta;
        }
    }

    for (size_t i = 0; i < moons; i++) {
        coords[i] += velocities[i];
    }
}

static int64_t total_energy(int64_t *coords, int64_t *velocities,
                            size_t moons) {
    int64_t e = 0;

    for (size_t i = 0; i < moons; i++) {
        e += (llabs(coords[i]) + llabs(coords[i + moons]) +
              llabs(coords[i + 2 * moons])) *
             (llabs(velocities[i]) + llabs(velocities[i + moons]) +
              llabs(velocities[i + 2 * moons]));
    }

    return e;
}

static void print_moons(int64_t *coords, int64_t *velocities, size_t moons) {
    for (size_t i = 0; i < moons; i++) {
        printf("pos = (%ld, %ld, %ld), vel = (%ld, %ld, %ld)\n", coords[i],
               coords[i + moons], coords[i + 2 * moons], velocities[i],
               velocities[i + moons], velocities[i + 2 * moons]);
    }
}

static size_t load_init_state(char *filename, int64_t **coords,
                              int64_t **velocities) {
    char *inp = file_read_full(filename);
    void *reg = regex_compile("<x=(-?\\d+), y=(-?\\d+), z=(-?\\d+)>");

    size_t amt_moons = 0;

    vec_t coords_vec = vec_create(sizeof(int64_t));
    regex_iter it = regex_iter_create(reg, inp);

    for (size_t captures[8]; regex_next_match(&it, captures);) {
        for (size_t i = 1; i <= 3; i++) {
            int64_t r = atoll(inp + captures[2 * i]);
            vec_push(&coords_vec, &r);
        }

        amt_moons++;
    }

    regex_iter_free(it);

    *coords = malloc(sizeof(int64_t) * amt_moons * 3);

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < amt_moons; j++) {
            (*coords)[i * amt_moons + j] =
                ((int64_t *)coords_vec.data)[j * 3 + i];
        }
    }

    vec_free(&coords_vec);
    regex_free(reg);
    free(inp);

    *velocities = malloc(sizeof(int64_t) * amt_moons * 3);
    memset(*velocities, 0, sizeof(int64_t) * amt_moons * 3);

    return amt_moons;
}

void d12p1() {
    int64_t *coords, *velocities;
    size_t amt_moons =
        load_init_state("input/day12/input", &coords, &velocities);

    for (size_t i = 0; i < 1000; i++) {
        do_step(coords, velocities, amt_moons);
        do_step(coords + amt_moons, velocities + amt_moons, amt_moons);
        do_step(coords + amt_moons * 2, velocities + amt_moons * 2, amt_moons);
    }

    int64_t e = total_energy(coords, velocities, amt_moons);

    printf("Energy = %ld\n", e);

    free(coords);
    free(velocities);
}

void d12p2() {
    int64_t *coords, *velocities;
    size_t amt_moons =
        load_init_state("input/day12/input", &coords, &velocities);

    size_t bytes = sizeof(int64_t) * amt_moons * 3;

    int64_t *orig_coords = malloc(bytes), *orig_velocities = malloc(bytes);

    memcpy(orig_coords, coords, bytes);
    memcpy(orig_velocities, velocities, bytes);

    int64_t x_period = 0, y_period = 0, z_period = 0;

    do {
        x_period++;
        do_step(coords, velocities, amt_moons);
    } while (memcmp(coords, orig_coords, sizeof(int64_t) * amt_moons) ||
             memcmp(velocities, orig_velocities, sizeof(int64_t) * amt_moons));
    do {
        y_period++;
        do_step(coords + amt_moons, velocities + amt_moons, amt_moons);
    } while (memcmp(coords + amt_moons, orig_coords + amt_moons,
                    sizeof(int64_t) * amt_moons) ||
             memcmp(velocities + amt_moons, orig_velocities + amt_moons,
                    sizeof(int64_t) * amt_moons));
    do {
        z_period++;
        do_step(coords + amt_moons * 2, velocities + amt_moons * 2, amt_moons);
    } while (memcmp(coords + amt_moons * 2, orig_coords + amt_moons * 2,
                    sizeof(int64_t) * amt_moons) ||
             memcmp(velocities + amt_moons * 2, orig_velocities + amt_moons * 2,
                    sizeof(int64_t) * amt_moons));

    printf("%ld\n", lcm(lcm(x_period, y_period), z_period));

    free(orig_velocities);
    free(orig_coords);
    free(coords);
    free(velocities);
}
