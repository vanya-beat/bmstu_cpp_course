#include <stdio.h>
#include "int2str.h"

char* int2str(int num) {
    static char buf[12];
    int i = 0;
    long long number = num;
    int nega = 0;

    if (number == 0) return "0";
    if (number < 0) {nega = 1; number = -number;}

    while (number > 0) {
        buf[i++] = (number % 10) + '0';
        number /= 10;
    }

    if (nega) buf[i++] = '-';

    buf[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = temp;
    }

    return buf;
}
