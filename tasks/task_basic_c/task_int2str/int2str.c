#include <assert.h>
#include "int2str.h"
#include "stdio.h"

char* int2str(int num) {
    long long number = num;
    if (number == 0) {
        return "0";
    }

    static char str[15] = "";
    int znak = 1;

    if (number < 0) {
        znak = -1;
        number *= -1;
    }

    int n_len = 0;

    if (number > 0)
        n_len++;
    if (number > 10)
        n_len++;
    if (number > 100)
        n_len++;
    if (number > 1000)
        n_len++;
    if (number > 10000)
        n_len++;
    if (number > 100000)
        n_len++;
    if (number > 1000000)
        n_len++;
    if (number > 10000000)
        n_len++;
    if (number > 100000000)
        n_len++;
    if (number > 1000000000)
        n_len++;
    if (number > 10000000000)
        n_len++;

    if (znak == -1) {
        n_len++;
        str[0] = '-';
    }
    
    int digit = 0;
    int count = 0;
    char ch;
    while (number > 0) {
        digit = number % 10;
        number = number / 10;
        ch = digit + '0';
        str[n_len - 1 - count] = ch;
        count++;
    }
    str[n_len] = '\0';

    return str;
}
