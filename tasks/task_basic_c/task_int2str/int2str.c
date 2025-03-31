#include <stdio.h>
#include <stdlib.h>
#include "int2str.h"
#include <assert.h>
#include <string.h>

char* int2str(int number) {
    char* str = (char *)calloc(12, sizeof(char));

    if (str) {
        int isNegative = (number < 0);
        unsigned int num = isNegative ? -number : number;
        int i = 11;
        
        str[i--] = '\0';

        if (number == 0) {
            str[i--] = '0';
        } else {
            while (num > 0) {
                str[i--] = (num % 10) + '0';
                num /= 10;
            }
        }

        if (isNegative) {
            str[i--] = '-';
        }

        int startIndex = i + 1;
        memmove(str, &str[startIndex], 12 - startIndex);
    }

    return str;
}