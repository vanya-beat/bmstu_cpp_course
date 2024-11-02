#include "int2str.h"
#include <stdlib.h>
#include <stdio.h>

char* int2str(int number) {
    char* str = malloc(12);
    int i = 10;
    unsigned int abs_number = (number < 0) ? -number : number;
    str[11] = '\0';

    if (number == 0) {
        str[i--] = '0';
    } else {
        while (abs_number > 0) {
            str[i--] = (abs_number % 10) + '0';
            abs_number /= 10;
        }

        if (number < 0) {
            str[i--] = '-';
        }
    }

    return &str[i + 1];
}
