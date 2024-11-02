#include <assert.h>
#include "int2str.h"
#include "stdio.h"
#include <stdbool.h>
#include <malloc.h>

char* int2str(int number) {
    int i = 0; //объявил переменную счета в начале, для удобства при реверсе массива 
    bool isNegative = false; //флажок для знака 
    char *str = (char*)malloc(13 * sizeof(char)); //выделяем память, чтобы точно хватило
    unsigned int num = (unsigned int)((number < 0) * -number) + (number >= 0) * number;
    
    if (number < 0) { //определят +/-
        isNegative = true;
    }
    str[i++] = (num % 10) + '0';  //разбираем число о добавляем в массив 
    num /= 10;        
    while (num > 0) {
        str[i++] = (num % 10) + '0';  //разбираем число о добавляем в массив 
        num /= 10; 
    }

    if (isNegative) {
        str[i++] = '-';                     
    }

    str[i] = '\0';    //закрываем массив                 

    for (int j = 0; j < i / 2; j++) {  //реверс массива 
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
    
    return str; //возвращаем массив
}