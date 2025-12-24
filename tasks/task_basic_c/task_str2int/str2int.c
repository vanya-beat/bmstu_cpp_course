#include "str2int.h"
#include <assert.h>
#include <stdio.h>

int str2int(const char* str) 
{
    assert(str != NULL);
    
    if (*str == '\0') {
        printf("Пустая строка\n");
        return 0;
    }
    
    int result = 0;
    int sign = 1;
    
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    if (*str != '\0') {
        printf("Нет цифр\n");
    }
    
    return result * sign;
}