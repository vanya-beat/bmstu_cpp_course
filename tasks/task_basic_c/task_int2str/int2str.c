#include <assert.h>
#include "int2str.h"
#include "stdio.h"
#include <malloc.h>

char* int2str(int number) {
    int i = 0;
    int isNegative = 0;
    char* str = "0";
    str = (char*)malloc(1000000 * sizeof(char));
    unsigned int num = (unsigned int)((number < 0) * -number) + (number >= 0) * number;

    if (number < 0) {
        isNegative = 1;
    }

    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    if (isNegative==1) {
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