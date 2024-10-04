#include <assert.h>
#include "str2int.h"
#include <stdio.h>
#include <stdbool.h>


bool is_space(char c) {
  return (c == ' ');
}
bool is_digit(char c) {
  return (c >= '0' && c <= '9');
}


int str2int(const char *str) {
  if (*str == '-') {
    assert(1 == 0);

  }

  int result = 0;
  int sign = 1;

  while (is_space(*str)) {
    str++;
  }

  if (*str == '-' || *str == '+') {
    sign = (*str == '-') ? -1 : 1;
    str++;
  }

  while (is_digit(*str)) {
    int digit = *str - 0x30;
    result = result * 10 + digit;
    str++;
  }
  return result * sign;
}
