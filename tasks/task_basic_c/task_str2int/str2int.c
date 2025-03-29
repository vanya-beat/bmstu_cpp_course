#include <assert.h>
#include "str2int.h"
#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "stdlib.h"

int str2int(const char *str) {
    int number = 0;
    int sign=1;
    int i;
    for (i=0; str[i]!='\0';i++) {
        switch (str[i])  {
            case '-' : sign=-1; break;
            case '0': number = number*10+0; break;
            case '1': number = number*10+1; break;
            case '2': number = number*10+2; break;
            case '3': number = number*10+3; break;
            case '4': number = number*10+4; break;
            case '5': number = number*10+5; break;
            case '6': number = number*10+6; break;
            case '7': number = number*10+7; break;
            case '8': number = number*10+8; break;
            case '9': number = number*10+9; break;
            default: break;
        }
    }
    number = number * sign;
    return number;
    return 0;
}