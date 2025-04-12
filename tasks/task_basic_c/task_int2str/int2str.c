#include <assert.h>
#include "int2str.h"
#include "stdio.h"
#include <string.h>
#include <malloc.h>
char * int2str(int number) {
    char * str = (char *)malloc(13*sizeof(char));
    int num = ((number <0) * -number)+(number >= 0) * number;
    int i = 0;

    if (num == 0){
        str[i++] = '0';
    }
    int sign = (number >=0 ? 1 : -1);
    number *= sign;

    while (number > 0){
        str[i++] = number % 10 + '0';
        number /= 10;
    }
    if (sign < 0) str[i++] = '-';
        str[i] = '\0';
    for (int j = 0; j < i / 2 ; j++){
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1 ] = temp;
        }
        return str;
    }
