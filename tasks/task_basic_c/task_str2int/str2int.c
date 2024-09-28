#include "assert.h"
#include "str2int.h"
#include "stdio.h"
// #include "stdlib.h"

// int asserting() {
//     assert(1 == 0);
//     return 0;
// }

int str2int(const char *str) {
    int result = 0;
    int sign = 1;

    // пока пробелы идём дальше
    while (*str == ' ') {
        str++;
    }

    if (*str == '-' || *str == '+') {
        sign = (*str == '-') ? -1 : 1;
        str++;
    }
    int digits = 0;
    while (*str >= '0' && *str <= '9') {
        if (*str >= '0' && *str <= '9') {
            int digit = *str - 0x30;
            result = result * 10 + digit;
            digits++;
        } else {
        }
        str++;
    }
    // printf("result is %d", result);
    if (digits == 0) {
        // asserting();
        assert(digits > 0);
        // digits = -1;
        // assert(digits > 0); {
    } else if (result < 0) {
        // exit(-1);
        // asserting();
        // assert(result > 0);
    }

    result = result * sign;
    return result;
}

// int main(int argc, char const *argv[])
// {
//     printf("%d", str2int("-"));
//     return 0;
// }

