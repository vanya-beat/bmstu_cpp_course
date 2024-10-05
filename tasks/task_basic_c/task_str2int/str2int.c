#include <assert.h>
#include "str2int.h"
#include "stdio.h"

int str2int(const char *str) {
    int znak = 1;
    int result = 0;

    if (*str == '-'){
        znak = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    int count = 0;
    while (*str != '\0') {
        result = result * 10 + (*str - '0');
        count += 1;
        str++;
        assert(znak == -1 && result == -2147483648 || znak == -1 && result >= 0 && count <= 9 || znak == 1 && result >= 0);
    }
    assert(count > 0);
    return result * znak;
}
