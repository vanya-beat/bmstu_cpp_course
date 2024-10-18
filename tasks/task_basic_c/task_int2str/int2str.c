#include <assert.h>
#include "int2str.h"
#include "stdio.h"
#include <malloc.h>

char* int2str(int number) {
    int i = 0; //объявил переменную счета в начале, для удобства при реверсе массива 
    int isNegative = 0; //флажок для знака 
    char *str = "0"; //переменная строки
    str = (char*)malloc(1000000 * sizeof(char)); //выделяем память, чтобы точно хватило
    unsigned int num = (unsigned int)((number < 0) * -number) + (number >= 0) * number; //с костылями избегаем переполнения number

    if (number < 0) { //определят +/-
        isNegative = 1;
    }

    do {
        str[i++] = (num % 10) + '0';  //разбираем число о добавляем в массив 
        num /= 10;                       
    } while (num > 0);

    if (isNegative==1) {
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
