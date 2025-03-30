#include "str2int.h"
#include <assert.h>
#include "stdio.h"



int str2int(const char* str)
{
    if (str == NULL)
    {
        return 0;
    }
    int number = 0;
    int sign=1; // знак числа
    int i;	
    bool flag=false; // проверка границ типа int от −2 147 483 648 до 2 147 483 647
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
        if (number == 214748364 && str[i]!='\0') // перед границей int
        {
            flag = true;
        }
        if (flag == true && (( str[i] == '8' && sign ==1) || str[i] == '9')) 
        {
            return 0; // если выходит за границы диапазона - возвращаем 0
        }
    }
    number = number * sign;
    return number;
}