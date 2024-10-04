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
  int sign = 1;
  if (*str == '\0') {
    assert(1 == 0);
  }


  if (*str == '-') {
    str++;
    sign=-1;
    if (*str == '\0') {
      assert(1 == 0);
    }
  } else if (*str == '+') {
    str++;
  }

  int result = 0;
  while (is_space(*str)) {
    str++;
  }



  while (is_digit(*str)) {
    int digit = *str - 0x30;
    result = result * 10 + digit;
    str++;
  }


  return sign * result;
}
