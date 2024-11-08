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
int main(void) {
  
    char str[25];
    printf("Введите строку\n"); 
    scanf("%s", str);
    int vaidation=1;
    for (int j=0; j<strlen(str);j++) {
       if (str[j]!='0' && 
       str[j]!='1' && str[j]!='2' && str[j]!='3' && 
       str[j]!='4' && str[j]!='5' && str[j]!='6' && 
       str[j]!='7' && str[j]!='8' && str[j]!='9') {
         if (j==0 && str[j]=='-') vaidation=1;
         else            vaidation=0;
       }
     }

     if (vaidation==1) {
           printf("%d\n", str2int(str));
     }
     else printf ("Введена некоррктная строка\n");
   }