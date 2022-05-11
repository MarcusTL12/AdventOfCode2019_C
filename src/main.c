#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "days.h"

static void print_time(clock_t diff) {
    clock_t secs = diff / CLOCKS_PER_SEC, rem = diff % CLOCKS_PER_SEC;
    double ms = ((double)(rem * 1000)) / ((double)CLOCKS_PER_SEC);
    printf("Took %ld s, %f ms\n", secs, ms);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Enter day and part as command line arguments,\n");
        printf("Or run all by passing \"all\" as parameter.\n");
    } else if (argc == 2) {
        if (*argv[1] == 'a') {
            printf("Running all days:\n");
            printf("===========================\n");
            clock_t timer = clock();
            for (size_t i = 0; i < AMT_DAYS; i++) {
                clock_t subtimer = clock();
                printf("---------------------------\n");
                printf("Running Day %zu\n", i + 1);
                printf("Part 1:\n");
                days[2 * i]();
                print_time(clock() - subtimer);
                printf("\n");

                subtimer = clock();
                printf("Part 2:\n");
                days[2 * i + 1]();
                print_time(clock() - subtimer);
            }
            printf("===========================\n");
            print_time(clock() - timer);
        } else {
            size_t day = atoi(argv[1]) - 1;

            if (day < 0 || day >= AMT_DAYS) {
                printf("Must give day between 1 and %d!\n", AMT_DAYS);
                return 0;
            }

            clock_t subtimer = clock();
            printf("Running Day %zu\n", day + 1);
            printf("Part 1:\n");
            days[2 * day]();
            print_time(clock() - subtimer);
            printf("\n");

            subtimer = clock();
            printf("Part 2:\n");
            days[2 * day + 1]();
            print_time(clock() - subtimer);
        }
    } else {
        int day = atoi(argv[1]) - 1;
        int part = atoi(argv[2]) - 1;

        if (day < 0 || day >= AMT_DAYS) {
            printf("Must give day between 1 and %d!\n", AMT_DAYS);
            return 0;
        }

        if (part < 0 || part > 1) {
            printf("Part must be either 1 or 2\n");
            return 0;
        }

        int index = 2 * day + part;

        clock_t start = clock();
        days[index]();
        print_time(clock() - start);
    }
}
