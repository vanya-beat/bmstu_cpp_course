#include <assert.h>
#include <stdlib.h>
#include "int2str.h"
#include "stdio.h"

char* int2str(int number) {
  char* str = malloc(12);
  int i = 0;
  int sign = number;

  if (number == 0) {
    return "0";
  }
  if (number == -2147483648) {
    return "-2147483648";
  }
  if (number == -0) {
    return "0";
  }
  if (number < 0) {
    number = -number;
  }

  while (number > 0) {
    str[i++] = number % 10 + '0';
    number /= 10;
  }

  if (sign < 0) {
    str[i++] = '-';
  }

  str[i] = '\0';
  for (int j = 0, k = i - 1; j < k; j++, k--) {
    char temp = str[j];
    str[j] = str[k];
    str[k] = temp;
  }

  return str;
}
