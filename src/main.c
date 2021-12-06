#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "days.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Enter day and part as command line arguments!\n");
    } else {
        int day = atoi(argv[1]) - 1;
        int part = atoi(argv[2]) - 1;

        if (day < 0 || day > 24) {
            printf("Must give day between 1 and 25!\n");
            return 0;
        }

        if (part < 0 || part > 1) {
            printf("Part must be either 1 or 2\n");
            return 0;
        }

        int index = 2 * day + part;

        clock_t start = clock();
        days[index]();
        clock_t diff = clock() - start;
        clock_t secs = diff / CLOCKS_PER_SEC, rem = diff % CLOCKS_PER_SEC;
        double ms = ((double)(rem * 1000)) / ((double)CLOCKS_PER_SEC);
        printf("Took %ld s, %f ms\n", secs, ms);
    }
}
