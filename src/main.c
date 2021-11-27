#include <stdio.h>
#include <stdlib.h>

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

        days[index]();
    }
}
