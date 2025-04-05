#include "int2str.h"
#include <assert.h>
#include <stdio.h>
#include <limits.h> 

void int2str(int num, char* str) 
{
    int i = 0;
    int is_negative = 0;
    
    
    if (num == INT_MIN) {
        str[0] = '-';
        str[1] = '2';
        str[2] = '1';
        str[3] = '4';
        str[4] = '7';
        str[5] = '4';
        str[6] = '8';
        str[7] = '3';
        str[8] = '6';
        str[9] = '4';
        str[10] = '8';
        str[11] = '\0';
        return;
    }

    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

 
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
