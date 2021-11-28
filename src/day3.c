#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../hashmap.c/hashmap.h"

typedef struct {
    int x, y, steps;
} point;

static int point_cmp(const void *a, const void *b, void *udata) {
    return memcmp(a, b, sizeof(int) * 2);
}

static uint64_t point_hash(const void *p, uint64_t s0, uint64_t s1) {
    return hashmap_sip(p, sizeof(int) * 2, s0, s1);
}

static int dist(point p) { return abs(p.x) + abs(p.y); }

static point get_dir(char c) {
    switch (c) {
        case 'R':
            return (point){1, 0};
        case 'U':
            return (point){0, 1};
        case 'L':
            return (point){-1, 0};
        case 'D':
            return (point){0, -1};

        default:
            return (point){0, 0};
    }
}

void d3p1() {
    char *inp = file_read_full("input/day3/input");

    struct hashmap *pts =
        hashmap_new(sizeof(point), 0, 0, 0, point_hash, point_cmp, NULL, NULL);

    hashmap_set(pts, &(point){0, 0});

    char *c = inp;

    {
        point pos = {0, 0};

        while (true) {
            point d = get_dir(*c++);
            int amt = atoi(c);

            for (int i = 0; i < amt; i++) {
                pos.x += d.x;
                pos.y += d.y;

                hashmap_set(pts, &pos);
            }

            while (isdigit(*c)) c++;
            if (*c++ != ',') break;
        }
    }

    {
        point pos = {0, 0};
        int closest = INT32_MAX;

        while (true) {
            point d = get_dir(*c++);
            int amt = atoi(c);

            for (int i = 0; i < amt; i++) {
                pos.x += d.x;
                pos.y += d.y;

                if (hashmap_get(pts, &pos)) {
                    int dst = dist(pos);
                    if (dst < closest) closest = dst;
                }
            }

            while (isdigit(*c)) c++;
            if (*c++ != ',') break;
        }

        printf("Closest: %i\n", closest);
    }

    hashmap_free(pts);

    free(inp);
}

void d3p2() {
    char *inp = file_read_full("input/day3/input");

    struct hashmap *pts =
        hashmap_new(sizeof(point), 0, 0, 0, point_hash, point_cmp, NULL, NULL);

    hashmap_set(pts, &(point){0, 0, 0});

    char *c = inp;

    {
        point pos = {0, 0, 0};

        while (true) {
            point d = get_dir(*c++);
            int amt = atoi(c);

            for (int i = 0; i < amt; i++) {
                pos.x += d.x;
                pos.y += d.y;
                pos.steps++;

                hashmap_set(pts, &pos);
            }

            while (isdigit(*c)) c++;
            if (*c++ != ',') break;
        }
    }

    {
        point pos = {0, 0, 0};
        int closest = INT32_MAX;

        while (true) {
            point d = get_dir(*c++);
            int amt = atoi(c);

            for (int i = 0; i < amt; i++) {
                pos.x += d.x;
                pos.y += d.y;
                pos.steps++;

                if (pos.steps > closest) break;

                point *p = hashmap_get(pts, &pos);

                if (p) {
                    int steps = p->steps + pos.steps;
                    if (steps < closest) closest = steps;
                }
            }

            if (pos.steps > closest) break;

            while (isdigit(*c)) c++;
            if (*c++ != ',') break;
        }

        printf("Closest: %i\n", closest);
    }

    hashmap_free(pts);

    free(inp);
}
