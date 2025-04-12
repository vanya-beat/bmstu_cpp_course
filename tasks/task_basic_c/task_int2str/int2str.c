#include <assert.h>
#include <malloc.h>
#include "int2str.h"
#include "stdio.h"

char* int2str(int number) {
    char* str = malloc(12 * sizeof(char));

    unsigned int num = (unsigned int)((number < 0) * -number) + (number >= 0) * number;
    int j = 12;

    str[12] = '\0';
    if (num == 0) {
        str[j] = '0';
        return str + j;
    }
    while (num > 0) {
        str[j] = (num % 10) + '0';
        num /= 10;
        j--;
    }
    if (number < 0) {
        str[j] = '-';
        j--;
    }

    return str + j + 1;
}