#include <stdio.h>

#include "intcode.h"

void d23p1() {
    intcode_machine machines[50];
    machines[0] = intcode_from_file("input/day23/input");
    for (size_t i = 1; i < 50; i++) {
        machines[i] = intcode_clone(&machines[0]);
    }

    for (size_t i = 0; i < 50; i++) {
        intcode_send(&machines[i], i);
    }

    for (size_t i = 0;; i = (i + 1) % 50) {
        intcode_send(&machines[i], -1);
        intcode_run(&machines[i]);
        for (int64_t packet[3];
             intcode_recieve_multiple(&machines[i], packet, 3);) {
            if (packet[0] == 255) {
                printf("%li\n", packet[2]);
                goto outer;
            }
            intcode_send(&machines[packet[0]], packet[1]);
            intcode_send(&machines[packet[0]], packet[2]);
        }
    }

outer:

    for (size_t i = 0; i < 50; i++) {
        intcode_free(&machines[i]);
    }
}

void d23p2() {
    intcode_machine machines[50];
    machines[0] = intcode_from_file("input/day23/input");
    for (size_t i = 1; i < 50; i++) {
        machines[i] = intcode_clone(&machines[0]);
    }

    for (size_t i = 0; i < 50; i++) {
        intcode_send(&machines[i], i);
    }

    int64_t nat_packet[2], prev_y = 0;

    for (size_t i = 0, idle = 0;; i = (i + 1) % 50, idle++) {
        if (!i && idle >= 50) {
            if (nat_packet[1] == prev_y) {
                printf("%li\n", prev_y);
                goto outer;
            }
            prev_y = nat_packet[1];
            intcode_send(&machines[i], nat_packet[0]);
            intcode_send(&machines[i], nat_packet[1]);
        }
        intcode_send(&machines[i], -1);
        intcode_run(&machines[i]);
        for (int64_t packet[3];
             intcode_recieve_multiple(&machines[i], packet, 3);) {
            idle = 0;
            if (packet[0] == 255) {
                nat_packet[0] = packet[1];
                nat_packet[1] = packet[2];
            } else {
                intcode_send(&machines[packet[0]], packet[1]);
                intcode_send(&machines[packet[0]], packet[2]);
            }
        }
    }

outer:

    for (size_t i = 0; i < 50; i++) {
        intcode_free(&machines[i]);
    }
}
