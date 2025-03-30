#include "int2str.h"
#include <assert.h>
#include "stdio.h"
#include <stdlib.h>

char* int2str(int number)
{
    int is_negative = number < 0;
    unsigned int n = is_negative ? -((unsigned int)number) : number;
    
    int length = 0;
    unsigned int temp = n;
    do {
        length++;
        temp /= 10;
    } while (temp > 0);
    
    if (is_negative) length++;
    
    char* str = (char*)malloc(length + 1);
    if (!str) return NULL;
    
    str[length] = '\0';
    
    for (int i = length - 1; i >= (is_negative ? 1 : 0); i--) {
        str[i] = '0' + (n % 10);
        n /= 10;
    }
    
    if (is_negative) str[0] = '-';
    if (length == 1 && str[0] == '0' && number == 0) return str;
    
    return str;
}