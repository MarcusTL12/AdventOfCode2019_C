#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "aoc_util/bitfield.h"
#include "aoc_util/deque.h"
#include "aoc_util/file_util.h"
#include "aoc_util/hashmap.h"
#include "aoc_util/point.h"
#include "aoc_util/show.h"

static void load_maze(char *filename, char **maze, size_t *w, size_t *h,
                      point2i *keys, point2i *doors, size_t *amt_keys,
                      point2i *start) {
    *maze = file_read_full(filename);

    *w = 0;
    for (char *c = *maze; *c != '\n'; c++) (*w)++;
    (*w)++;
    *h = (strlen(*maze) + 1) / (*w);

    *amt_keys = 0;

    for (size_t y = 0; y < *h; y++) {
        for (size_t x = 0; x < *w; x++) {
            char c = (*maze)[y * *w + x];

            if (isalpha(c)) {
                point2i *v = islower(c) ? keys : doors;
                size_t ind = c & 0b11111;
                v[ind - 1] = (point2i){x, y};
                if (islower(c)) {
                    *amt_keys = ind > *amt_keys ? ind : *amt_keys;
                }
            } else if (c == '@') {
                *start = (point2i){x, y};
                (*maze)[y * *w + x] = '.';
            }
        }
    }
}

typedef struct {
    point2i pos;
    int64_t data;
} cell_t;

const static point2i DIRS[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

static void find_keys_bfs(char *maze, size_t w, point2i start, deque_t *queue,
                          point2i *keys, uint64_t collected_keys,
                          struct hashmap *visited) {
    deque_clear(queue);
    hashmap_clear(visited, true);

    deque_push_back(queue, &(cell_t){start, 0});
    hashmap_set(visited, &start);

    size_t amt_keys = 0;

    for (cell_t curcell; deque_pop_front(queue, &curcell);) {
        for (size_t i = 0; i < 4; i++) {
            cell_t nextcell = {point2i_add(curcell.pos, DIRS[i]),
                               curcell.data + 1};
            char c = maze[nextcell.pos.y * w + nextcell.pos.x];
            if (c != '#' && !hashmap_get(visited, &nextcell)) {
                if (c == '.' || isalpha(c) && bitfield_get(&collected_keys,
                                                           (c & 0b11111) - 1)) {
                    deque_push_back(queue, &nextcell);
                    hashmap_set(visited, &nextcell);
                } else if (isalpha(c) && islower(c) &&
                           !bitfield_get(&collected_keys, (c & 0b11111) - 1)) {
                    hashmap_set(visited, &nextcell);
                    keys[amt_keys++] =
                        (point2i){(c & 0b11111) - 1, nextcell.data};
                }
            }
        }
    }
}

typedef struct {
    point2i pos;
    uint64_t collected_keys;
    size_t steps;
} memo_t;

static size_t find_shortest_rec(char *maze, size_t w, point2i start,
                                uint64_t collected_keys, deque_t *queue,
                                struct hashmap *visited, struct hashmap *memo,
                                point2i *key_pos) {
    {
        memo_t *save = hashmap_get(memo, &(memo_t){start, collected_keys, 0});
        if (save) {
            return save->steps;
        }
    }

    point2i keys[26] = {[0 ... 25] = {0, 0}};
    find_keys_bfs(maze, w, start, queue, keys, collected_keys, visited);

    size_t min_ind = 0;
    size_t min_steps = -1;
    uint64_t new_collected;
    for (size_t i = 0; keys[i].y; i++) {
        new_collected = collected_keys;
        bitfield_set(&new_collected, keys[i].x, true);
        size_t cur_steps =
            keys[i].y + find_shortest_rec(maze, w, key_pos[keys[i].x],
                                          new_collected, queue, visited, memo,
                                          key_pos);

        if (cur_steps < min_steps) {
            min_steps = cur_steps;
            min_ind = i;
        }
    }

    if (min_steps == -1) {
        min_steps = 0;
    }

    hashmap_set(memo, &(memo_t){start, collected_keys, min_steps});

    return min_steps;
}

void d18p1() {
    char *maze;
    size_t w, h, amt_keys;
    point2i keys[26], doors[26], start;
    load_maze("input/day18/input", &maze, &w, &h, keys, doors, &amt_keys,
              &start);

    deque_t queue = deque_create(sizeof(cell_t));
    struct hashmap *visited = hashmap_new(
        sizeof(point2i), 0, 18, 1, point2i_hash, point2i_cmp, NULL, NULL);
    struct hashmap *memo = hashmap_new(sizeof(memo_t), 0, 2019, 18,
                                       point3i_hash, point3i_cmp, NULL, NULL);

    size_t ans =
        find_shortest_rec(maze, w, start, 0, &queue, visited, memo, keys);

    printf("%zu\n", ans);

    hashmap_free(memo);
    hashmap_free(visited);
    deque_free(&queue);
    free(maze);
}

void d18p2() {}