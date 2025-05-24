#include "str2int.h"
#include <assert.h>
#include "stdio.h"
#include <ostream>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

int str2int(const char* str) {
    if (str == NULL || *str == '\0') {
        abort(); 
    }

    int sign = 1;
    long long result = 0;
    size_t i = 0;

    if (str[i] == '-') {
        sign = -1;
        i++;
        if (str[i] == '\0') abort(); 
    } else if (str[i] == '+') {
        i++;
        if (str[i] == '\0') abort(); 
    }

    
    // if (sign == -1 && strcmp(str+i, "2147483648") == 0) {
    //     return INT_MIN;
    // }

   
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) abort();

        result = result * 10 + (str[i] - '0');

        
        if (sign == 1 && result > INT_MAX) abort();
        if (sign == -1 && result > (long long)INT_MAX + 1) abort();
    }

    result *= sign;

    
    if (result < INT_MIN || result > INT_MAX) abort();

    return (int)result;
}
