#include "int2str.h"

#include <stdio.h>
#include <stdlib.h>

char* int2str(int num) {
    char* buffer = calloc(12, sizeof(char));
    unsigned int positive_num = (num < 0) ? -num : num;
    buffer[11] = '\0';
    int index = 10;

    if (num == 0) {
        buffer[index] = '0';
        index--;
    } else {
        while (positive_num > 0) {
            buffer[index] = (positive_num % 10) + '0';
            positive_num /= 10;
            index--;
        }
        if (num < 0) {
            buffer[index] = '-';
            index--;
        }
    }
    return &buffer[index + 1];
}
