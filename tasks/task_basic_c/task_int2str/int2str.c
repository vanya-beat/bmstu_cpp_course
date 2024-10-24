// #include <assert.h>
// #include "int2str.h"
// #include "stdio.h"

// char* int2str(int number) {
//     static char str[12];
//     int i = 0, is_negative = 0;
//     unsigned int num;

//     if (number == 0) {
//         str[0] = '0';
//         str[1] = '\0';
//         return str;
//     }

//     if (number < 0) {
//         is_negative = 1;
//         num = (number == -2147483648) ? 2147483648 : (unsigned int)(-number);
//     } else {
//         num = number;
//     }

//     while (num > 0) {
//         str[i++] = num % 10 + '0';
//         num /= 10;
//     }

//     if (is_negative)
//         str[i++] = '-';

//     str[i] = '\0';

//     for (int j = 0; j < i / 2; j++) {
//         char temp = str[j];
//         str[j] = str[i - 1 - j];
//         str[i - 1 - j] = temp;
//     }

//     return str;
// }
#include "int2str.h"
#include <limits.h>
#include <stdlib.h>

char* int2str(int num) {
    if (num == 0) {
        return "0";
    }

    if (num == INT_MIN) {
        return "-2147483648";
    }
    
    char* str = calloc(13, sizeof(char));
    int num_len = 0;

    if (num < 0) {
        num_len++;
        str[0] = '-';
        num *= -1;
    }

    int num_double = num;
    while (num_double > 0) {
        num_double /= 10;
        num_len++;
    }

    int numeral = 0;
    int c = 0;
    char trn = 0;
    while (num > 0) {
        numeral = num % 10;
        num = num / 10;
        trn = numeral + '0';
        str[num_len - 1 - c] = trn;
        c++;
    }
    str[num_len] = '\0';

    return str;
}
