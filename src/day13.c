#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "intcode.h"

void d13p1() {
    intcode_machine machine = intcode_from_file("input/day13/input");

    intcode_run(&machine);

    size_t ans = 0;

    for (int64_t buf[3]; intcode_recieve_multiple(&machine, buf, 3);) {
        ans += buf[2] == 2 ? 1 : 0;
    }

    printf("%zu\n", ans);

    intcode_free(&machine);
}

static char *get_board(intcode_machine *machine, size_t *width_,
                       size_t *height_, int64_t *ball_x, int64_t *paddle_x) {
    intcode_run(machine);

    vec_t commands = vec_create(sizeof(int64_t) * 3);

    size_t width = 0, height = 0;

    for (int64_t buf[3]; intcode_recieve_multiple(machine, buf, 3);) {
        if (buf[0] == -1) break;
        if (buf[2] == 4) *ball_x = buf[0];
        if (buf[2] == 3) *paddle_x = buf[0];
        vec_push(&commands, buf);
        width = buf[0] > width ? buf[0] : width;
        height = buf[1] > height ? buf[1] : height;
    }

    width += 1;
    height += 1;

    char *board = malloc(width * height);

    for (size_t i = 0; i < commands.len; i++) {
        int64_t *buf = vec_get(&commands, i);

        board[buf[0] + buf[1] * width] = buf[2];
    }

    vec_free(&commands);

    *width_ = width;
    *height_ = height;

    return board;
}

static void print_board(char *buffer, char *board, size_t width, size_t height,
                        int64_t score) {
    char *c = buffer;
    char symbols[] = {' ', '#', '_', '-', '*'};
    c += sprintf(c, "Score: %10ld\n", score);
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            c += sprintf(c, "%c", symbols[board[width * i + j]]);
        }
        c += sprintf(c, "\n");
    }
    puts(buffer);
}

static int64_t update_board(char *board, intcode_machine *machine, size_t width,
                            int64_t *score) {
    int64_t ball_x;

    for (int64_t buf[3]; intcode_recieve_multiple(machine, buf, 3);) {
        if (buf[0] == -1) {
            *score = buf[2];
        } else {
            board[buf[0] + width * buf[1]] = buf[2];
            if (buf[2] == 4) {
                ball_x = buf[0];
            }
        }
    }

    return ball_x;
}

#define animate

void d13p2() {
#ifdef animate
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 10000000;
#endif

    intcode_machine machine = intcode_from_file("input/day13/input");

    ((int64_t *)(machine.program.data))[0] = 2;

    size_t width, height;
    int64_t ball_x, paddle_x;
    char *board = get_board(&machine, &width, &height, &ball_x, &paddle_x);

    int64_t score = 0;

#ifdef animate
    char *buffer = malloc(width * height * 4 + 100);
    print_board(buffer, board, width, height, score);
    nanosleep(&ts, NULL);
#endif

    while (!machine.halted) {
        int64_t padd_ball_diff = ball_x - paddle_x;

        int64_t paddle_move = padd_ball_diff ? padd_ball_diff > 0 ? 1 : -1 : 0;

        paddle_x += paddle_move;
        intcode_send(&machine, paddle_move);
        intcode_run(&machine);
        ball_x = update_board(board, &machine, width, &score);

#ifdef animate
        printf("\x1b[999D\x1b[%zuA", height + 2);
        print_board(buffer, board, width, height, score);
        nanosleep(&ts, NULL);
#endif
    }

    printf("Final score: %ld\n", score);

#ifdef animate
    free(buffer);
#endif
    free(board);
    intcode_free(&machine);
}
