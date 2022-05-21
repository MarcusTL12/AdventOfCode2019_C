#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "aoc_util/deque.h"
#include "aoc_util/file_util.h"
#include "aoc_util/hashmap.h"
#include "aoc_util/hashmap_util.h"
#include "aoc_util/point.h"

typedef struct {
    point2i from, to;
    int64_t direction;
} portal_t;

static bool is_outside(size_t w, size_t h, point2i p) {
    return p.x == 2 || p.y == 2 || p.x == w - 4 || p.y == h - 3;
}

static struct hashmap *find_portals(char *maze, size_t w, size_t h,
                                    point2i *start, point2i *end) {
    struct hashmap *portals = hashmap_new(
        sizeof(portal_t), 0, 2019, 20, point2i_hash, point2i_cmp, NULL, NULL);

    point2i queue[26 * 26] = {[0 ... 26 * 26 - 1] = {0, 0}};

    for (size_t y = 2; y < h - 2; y++) {
        for (size_t x = 2; x < w - 3; x++) {
            if (maze[x + w * y] == '.') {
                for (size_t i = 0; i < 4; i++) {
                    point2i p = point2i_add((point2i){x, y}, DIRS2i4[i]);
                    char c1 = maze[p.x + w * p.y];
                    if (isalpha(c1)) {
                        p = point2i_add(p, DIRS2i4[i]);
                        char c2 = maze[p.x + w * p.y];

                        if (!(i % 2)) {
                            char tmp = c1;
                            c1 = c2;
                            c2 = tmp;
                        }

                        size_t qind = (c1 - 'A') * 26 + c2 - 'A';
                        if (qind == 0) {
                            *start = (point2i){x, y};
                        } else if (qind == 26 * 26 - 1) {
                            *end = (point2i){x, y};
                        } else if (queue[qind].x) {
                            portal_t portal = {
                                {x, y},
                                queue[qind],
                                is_outside(w, h, (point2i){x, y}) ? -1 : 1};
                            hashmap_set(portals, &portal);
                            portal = (portal_t){portal.to, portal.from,
                                                -portal.direction};
                            hashmap_set(portals, &portal);
                        } else {
                            queue[qind] = (point2i){x, y};
                        }
                    }
                }
            }
        }
    }

    return portals;
}

typedef struct {
    point2i pos;
    int64_t floor;
    size_t steps;
} state_t;

void d20p1() {
    char *maze = file_read_full("input/day20/input");

    size_t w = 0, h;
    for (char *c = maze; *c != '\n'; c++) w++;
    w++;
    h = (strlen(maze) + 1) / w;

    point2i start, end;
    struct hashmap *portals = find_portals(maze, w, h, &start, &end);

    deque_t queue = deque_create(sizeof(state_t));
    struct hashmap *visited = hashmap_new(
        sizeof(point2i), 0, 20, 1, point2i_hash, point2i_cmp, NULL, NULL);

    deque_push_back(&queue, &(state_t){.pos = start, .steps = 0});
    hashmap_set(visited, &start);

    size_t ans;

    for (state_t state; deque_pop_front(&queue, &state);) {
        for (size_t i = 0; i < 4; i++) {
            state_t nstate = {.pos = point2i_add(state.pos, DIRS2i4[i]),
                              .steps = state.steps + 1};
            if (nstate.pos.x == end.x && nstate.pos.y == end.y) {
                ans = nstate.steps;
                goto found_exit;
            }
            if (maze[nstate.pos.x + w * nstate.pos.y] == '.' &&
                !hashmap_get(visited, &nstate.pos)) {
                deque_push_back(&queue, &nstate);
                hashmap_set(visited, &nstate.pos);
            }
        }

        portal_t *portal = hashmap_get(portals, &state.pos);
        if (portal) {
            state_t nstate = {.pos = portal->to, .steps = state.steps + 1};
            if (maze[nstate.pos.x + w * nstate.pos.y] == '.' &&
                !hashmap_get(visited, &nstate.pos)) {
                deque_push_back(&queue, &nstate);
                hashmap_set(visited, &nstate.pos);
            }
        }
    }

found_exit:

    printf("%zu\n", ans);

    hashmap_free(visited);
    deque_free(&queue);
    hashmap_free(portals);
    free(maze);
}

MAKE_HASH_AND_CMP(state, sizeof(point2i) + sizeof(int64_t))

void d20p2() {
    char *maze = file_read_full("input/day20/input");

    size_t w = 0, h;
    for (char *c = maze; *c != '\n'; c++) w++;
    w++;
    h = (strlen(maze) + 1) / w;

    point2i start, end;
    struct hashmap *portals = find_portals(maze, w, h, &start, &end);

    deque_t queue = deque_create(sizeof(state_t));
    struct hashmap *visited =
        hashmap_new(sizeof(point2i) + sizeof(int64_t), 0, 20, 2, state_hash,
                    state_cmp, NULL, NULL);

    deque_push_back(&queue, &(state_t){.pos = start, .floor = 0, .steps = 0});
    hashmap_set(visited, &(state_t){.pos = start, .floor = 0});

    size_t ans = 0;

    for (state_t state; deque_pop_front(&queue, &state);) {
        for (size_t i = 0; i < 4; i++) {
            state_t nstate = {.pos = point2i_add(state.pos, DIRS2i4[i]),
                              .floor = state.floor,
                              .steps = state.steps + 1};
            if (nstate.pos.x == end.x && nstate.pos.y == end.y &&
                !nstate.floor) {
                ans = nstate.steps;
                goto found_exit;
            }
            if (maze[nstate.pos.x + w * nstate.pos.y] == '.' &&
                !hashmap_get(visited, &nstate)) {
                deque_push_back(&queue, &nstate);
                hashmap_set(visited, &nstate);
            }
        }

        portal_t *portal = hashmap_get(portals, &state.pos);
        if (portal) {
            state_t nstate = {.pos = portal->to,
                              .floor = state.floor + portal->direction,
                              .steps = state.steps + 1};
            if (maze[nstate.pos.x + w * nstate.pos.y] == '.' &&
                nstate.floor >= 0 && !hashmap_get(visited, &nstate)) {
                deque_push_back(&queue, &nstate);
                hashmap_set(visited, &nstate);
            }
        }
    }

found_exit:

    printf("%zu\n", ans);

    hashmap_free(visited);
    deque_free(&queue);
    hashmap_free(portals);
    free(maze);
}
