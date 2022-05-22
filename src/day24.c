#include <stdint.h>
#include <stdio.h>

#include "aoc_util/bitfield.h"
#include "aoc_util/file_util.h"
#include "aoc_util/hashmap.h"
#include "aoc_util/hashmap_util.h"
#include "aoc_util/point.h"

static uint32_t load_input(char *filename) {
    char *input = file_read_full(filename);

    uint32_t board = 0;

    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            uint32_t bit = input[6 * i + j] == '#' ? 1 : 0;
            bit <<= 5 * i + j;
            board |= bit;
        }
    }

    free(input);

    return board;
}

MAKE_HASH_AND_CMP(uint32, sizeof(uint32_t))

static bool get_cell(uint32_t board, int64_t x, int64_t y) {
    return 0 <= x && x < 5 && 0 <= y && y < 5 ? board & (1 << (x + 5 * y))
                                              : false;
}

void d24p1() {
    uint32_t board = load_input("input/day24/input");

    struct hashmap *seen = hashmap_new(sizeof(uint32_t), 0, 24, 1, uint32_hash,
                                       uint32_cmp, NULL, NULL);

    while (!hashmap_get(seen, &board)) {
        hashmap_set(seen, &board);
        uint32_t new_board = board;
        for (int64_t y = 0; y < 5; y++) {
            for (int64_t x = 0; x < 5; x++) {
                size_t count = 0;
                for (size_t i = 0; i < 4; i++) {
                    point2i np = point2i_add((point2i){x, y}, DIRS2i4[i]);
                    count += get_cell(board, np.x, np.y) ? 1 : 0;
                }
                if (get_cell(board, x, y) && count != 1) {
                    new_board &= ~(1 << (x + 5 * y));
                } else if (!get_cell(board, x, y) &&
                           (count == 1 || count == 2)) {
                    new_board |= 1 << (x + 5 * y);
                }
            }
        }
        board = new_board;
    }

    printf("%u\n", board);

    hashmap_free(seen);
}

#define STEPS 200
#define DEPTH 201
#define MID DEPTH / 2
#define BUF_LEN (DEPTH * (25 * 25 - 1) + 64) / 64

static bool get_cell2(uint64_t *board, int64_t x, int64_t y, int64_t z) {
    if (x < 0 || x >= 5 || y < 0 || y >= 5 || z < 0 || z >= DEPTH) {
        return false;
    }

    size_t i = 24 * z;
    size_t j = x + 5 * y;
    if (j > 12) {
        j--;
    }

    return bitfield_get(board, i + j);
}

static void set_cell2(uint64_t *board, int64_t x, int64_t y, int64_t z,
                      bool v) {
    if (0 <= x && x < 5 && 0 <= y && y < 5 && 0 <= z && z < DEPTH) {
        size_t i = 24 * z;
        size_t j = x + 5 * y;
        if (j != 12) {
            if (j > 12) {
                j--;
            }
            bitfield_set(board, i + j, v);
        }
    }
}

static size_t count_adj(uint64_t *board, int64_t x, int64_t y, int64_t z) {
    if (x == 2 && y == 2) {
        return 0;
    }

    size_t c = 0;

    if (x == 0) {
        c += get_cell2(board, 1, 2, z - 1);
    } else if (x == 3 && y == 2) {
        for (size_t i = 0; i < 5; i++) {
            c += get_cell2(board, 4, i, z + 1);
        }
    } else {
        c += get_cell2(board, x - 1, y, z);
    }

    if (x == 4) {
        c += get_cell2(board, 3, 2, z - 1);
    } else if (x == 1 && y == 2) {
        for (size_t i = 0; i < 5; i++) {
            c += get_cell2(board, 0, i, z + 1);
        }
    } else {
        c += get_cell2(board, x + 1, y, z);
    }

    if (y == 0) {
        c += get_cell2(board, 2, 1, z - 1);
    } else if (x == 2 && y == 3) {
        for (size_t i = 0; i < 5; i++) {
            c += get_cell2(board, i, 4, z + 1);
        }
    } else {
        c += get_cell2(board, x, y - 1, z);
    }

    if (y == 4) {
        c += get_cell2(board, 2, 3, z - 1);
    } else if (x == 2 && y == 1) {
        for (size_t i = 0; i < 5; i++) {
            c += get_cell2(board, i, 0, z + 1);
        }
    } else {
        c += get_cell2(board, x, y + 1, z);
    }

    return c;
}

void d24p2() {
    uint32_t board_mid = load_input("input/day24/input");
    uint64_t buf1[BUF_LEN] = {0};
    uint64_t buf2[BUF_LEN];

    uint64_t *board = buf1, *nboard = buf2;

    for (size_t y = 0; y < 5; y++) {
        for (size_t x = 0; x < 5; x++) {
            set_cell2(board, x, y, MID, get_cell(board_mid, x, y));
        }
    }

    for (size_t i = 0; i < STEPS; i++) {
        memcpy(nboard, board, sizeof(buf1));

        for (int64_t z = 0; z < DEPTH; z++) {
            for (int64_t y = 0; y < 5; y++) {
                for (int64_t x = 0; x < 5; x++) {
                    if (x != 2 || y != 2) {
                        size_t c = count_adj(board, x, y, z);
                        if (get_cell2(board, x, y, z) && c != 1) {
                            set_cell2(nboard, x, y, z, false);
                        } else if (!get_cell2(board, x, y, z) &&
                                   (c == 1 || c == 2)) {
                            set_cell2(nboard, x, y, z, true);
                        }
                    }
                }
            }
        }

        {
            uint64_t *tmp = board;
            board = nboard;
            nboard = tmp;
        }
    }

    size_t ans = bitfield_count(board, BUF_LEN);

    printf("%zu\n", ans);
}
