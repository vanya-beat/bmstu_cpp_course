#include "int2str.h"
#include <limits.h>
#include <stdlib.h>

void convert_positive_number(char* str, long long num, int* pos) {
    int digits[10];      
    int digit_count = 0;
    
    while (num > 0) {
        digits[digit_count++] = num % 10;
        num /= 10;
    }
    
    while (digit_count > 0) {
        str[(*pos)++] = digits[--digit_count] + '0';
    }
}

char* int2str(int num) {
    char* str = calloc(12, sizeof(char));
    
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    int i = 0;
    
    if (num < 0) {
        str[i++] = '-';
    }

    long long abs_num = (num == INT_MIN) ? 
        -(long long)num : 
        (num < 0) ? -num : num;

    
    convert_positive_number(str, abs_num, &i);
    str[i] = '\0';
    
    return str;
}
