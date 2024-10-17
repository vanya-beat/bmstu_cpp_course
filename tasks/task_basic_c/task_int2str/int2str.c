#include <assert.h>
#include "int2str.h"
#include "stdio.h"

char* int2str(int number) {
    char *str="0";
    int i = 0;

    for (int num = number; num>0; num/=10){
        i++;
    }

    if (number<0){
        str[0] = '-';
        i++;
    }

    

    for ( ; i>0; i--){
        str[i]=number%10;
    }
    return str;
}
