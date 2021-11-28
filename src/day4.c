#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int LOW = 359282;
const int HIGH = 820401;

static bool isvalid1(char *digits) {
    bool has_pair = false;
    bool decrease = false;

    for (int i = 0; i < 5; i++) {
        has_pair |= digits[i] == digits[i + 1];
        decrease |= digits[i + 1] < digits[i];
    }

    return has_pair && !decrease;
}

static void increase(char *digits) {
    int i = 5;
    while (true) {
        if (digits[i] == '9') {
            digits[i--] = '0';
        } else {
            break;
        }
    }
    digits[i]++;
}

void d4p1() {
    int ans = 0;
    char digits[7];
    sprintf(digits, "%i", LOW);
    for (int i = 0; i <= HIGH - LOW + 1; i++) {
        ans += isvalid1(digits);
        increase(digits);
    }
    printf("%i\n", ans);
}

static bool isvalid2(char *d) {
    bool has_pair = ((d[0] == d[1]) & (d[1] != d[2])) |
                    ((d[0] != d[1]) & (d[1] == d[2]) & (d[2] != d[3])) |
                    ((d[1] != d[2]) & (d[2] == d[3]) & (d[3] != d[4])) |
                    ((d[2] != d[3]) & (d[3] == d[4]) & (d[4] != d[5])) |
                    ((d[3] != d[4]) & (d[4] == d[5]));
    bool notdecrease = (d[0] <= d[1]) & (d[1] <= d[2]) & (d[2] <= d[3]) &
                       (d[3] <= d[4]) & (d[4] <= d[5]);

    return has_pair & notdecrease;
}

void d4p2() {
    int ans = 0;
    char digits[7];
    sprintf(digits, "%i", LOW);
    for (int i = 0; i <= HIGH - LOW + 1; i++) {
        ans += isvalid2(digits);
        increase(digits);
    }
    printf("%i\n", ans);
}
