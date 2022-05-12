#include <stdio.h>

#include "aoc_util/hashmap.h"
#include "aoc_util/point.h"
#include "intcode.h"

typedef struct {
    point2i pos;
    bool iswall;
} cell;

static const point2i DIRS[] = {(point2i){0, -1}, (point2i){0, 1},
                               (point2i){-1, 0}, (point2i){1, 0}};
static const size_t DIRS_REV[] = {1, 0, 3, 2};

static void mapmaze_rec(intcode_machine *machine, struct hashmap *maze,
                        point2i pos, size_t *pathlen, point2i *target,
                        size_t *targetdist) {
    for (size_t dir = 0; dir < 4; dir++) {
        cell ncell = {point2i_add(pos, DIRS[dir]), false};
        if (!hashmap_get(maze, &ncell)) {
            intcode_send(machine, dir + 1);
            intcode_run(machine);
            int64_t response;
            intcode_recieve(machine, &response);
            switch (response) {
                case 0:
                    ncell.iswall = true;
                    hashmap_set(maze, &ncell);
                    break;
                case 2:
                    *target = ncell.pos;
                    *targetdist = *pathlen + 1;
                case 1:
                    hashmap_set(maze, &ncell);
                    (*pathlen)++;
                    mapmaze_rec(machine, maze, ncell.pos, pathlen, target,
                                targetdist);
                    (*pathlen)--;
                    intcode_send(machine, DIRS_REV[dir] + 1);
                    intcode_run(machine);
                    intcode_recieve(machine, &response);
                    break;
            }
        }
    }
}

static void mapmaze(intcode_machine *machine, struct hashmap **maze,
                    point2i *target, size_t *targetdist) {
    *maze = hashmap_new(sizeof(cell), 0, 2019, 15, point2i_hash, point2i_cmp,
                        NULL, NULL);

    cell curcell = {{0, 0}, false};
    hashmap_set(*maze, &curcell);

    size_t pathlen = 0;

    mapmaze_rec(machine, *maze, (point2i){0, 0}, &pathlen, target, targetdist);
}

void d15p1() {
    intcode_machine machine = intcode_from_file("input/day15/input");
    struct hashmap *maze;
    point2i target;
    size_t targetdist;

    mapmaze(&machine, &maze, &target, &targetdist);

    printf("Distance: %zu\n", targetdist);

    hashmap_free(maze);
    intcode_free(&machine);
}

void d15p2() {}
