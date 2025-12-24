#include "int2str.h"
#include <assert.h>
#include <stdio.h>

char* int2str(int number) {
    static char str[12]; // статический буфер
    int i = 0;
    int is_negative = 0;

    if (number == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    if (number < 0) {
        is_negative = 1;
        number = -number;
    }

    while (number > 0) {
        str[i++] = (number % 10) + '0';
        number /= 10;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    return str;
}