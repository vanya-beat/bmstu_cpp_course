#include <stdio.h> 
#include "stdbool.h" 
#include <assert.h> 
#include "str2int.h"
 
bool space(char c) { 
    return (c == ' '); 
} 
bool digit(char c) {     
    return (c >= '0' & c <= '9'); 
} 
int str2int(const char *str) { 
    int result = 0; 
    int sign = 1;
    bool flag = 1;
    while (space(*str)) { 
        str++;     
    } 
    if (*str == '-' || *str == '+') { 
        sign = (*str == '-') ? -1 : 1;         
        str++; 
    } 
    while (digit(*str)) {       
        int digit = *str - 0x30;  
        if (((sign == -1) && (result > (-2147483647 - 1 - digit) / 10)) || ((sign == 1) && (result > (2147483647 - digit) / 10))) { 
            assert(0==1); 
        } 
        result = result * 10 + digit;  
        flag = 0;      
        str++; 
    } 
    if (flag) {
        assert(0==1);
    }
    return result * sign; 
}
