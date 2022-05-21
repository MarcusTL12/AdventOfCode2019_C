#include <ctype.h>
#include <stdio.h>

#include "aoc_util/file_util.h"
#include "aoc_util/number_theory.h"
#include "aoc_util/point.h"

static point2i compose(point2i a, point2i b, int64_t n) {
    return (point2i){modmul(a.x, b.x, n), (modmul(a.x, b.y, n) + a.y) % n};
}

static point2i make_poly(char *filename, int64_t n) {
    char *inp = file_read_full(filename);

    point2i ans = {1, 0};

    for (char *c = inp; *c && *c != '\n';) {
        if (*c == 'c') {
            c += 4;
            int64_t x = atoll(c);
            while (*c && *c != '\n') c++;
            c++;

            ans = compose(ans, (point2i){1, x}, n);
        } else if (c[5] == 'w') {
            c += 20;
            int64_t x = atoll(c);
            while (*c && *c != '\n') c++;
            c++;

            ans = compose(ans, (point2i){invmod(x, n), 0}, n);
        } else {
            c += 20;

            ans = compose(ans, (point2i){-1, -1}, n);
        }
    }

    free(inp);

    return ans;
}

void d22p1() {
    int64_t n = 10007;
    point2i p = make_poly("input/day22/input", n);

    int64_t ans = modmul(2019 - p.y, invmod(p.x, n), n);

    if (ans < 0) ans += n;

    printf("%li\n", ans);
}

static point2i repeat(point2i p, int64_t n, int64_t m) {
    int64_t an = modexp(p.x, n, m);
    return (point2i){an, modmul(p.y, modmul(1 - an, invmod(1 - p.x, m), m), m)};
}

void d22p2() {
    int64_t n1 = 119315717514047, n2 = 101741582076661;

    point2i p = make_poly("input/day22/input", n1);

    point2i p2 = repeat(p, n2, n1);

    int64_t ans = (modmul(p2.x, 2020, n1) + p2.y) % n1;

    printf("%li\n", ans);
}
