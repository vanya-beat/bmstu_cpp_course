#include <stdio.h>

void intToString(int number, char *str) {
    int isNegative = 0;
    int i = 0;

    if (number < 0) {
        isNegative = 1;
        number = -number;
    }

    do {
        str[i++] = (number % 10) + '0';
        number /= 10;
    } while (number > 0);

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '0';

    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

int main(void) {
    int number = -12345;
    char str[50];

    intToString(number, str);

    printf("Число как строка: %s \n", str);

    return 0;
}
