#include <assert.h>
#include "int2str.h"
#include "stdio.h"
#include <malloc.h>

char* int2str(int number) {
    int i = 0;
    int isNegative = 0;
    char *str = "0";
    str = (char*)malloc(20 * sizeof(char));

    if (number < 0) {
        isNegative = 1;
        number = -number;
    }

    do {
        str[i++] = (number % 10) + '0';  
        number /= 10;                       
    } while (number > 0);

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
