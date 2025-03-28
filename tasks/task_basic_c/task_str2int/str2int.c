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
    int poly4aetca = 0; 
    int znak = 1;
    bool base = 1;
    while (space(*str)) { 
        str++;     
    } 
    if (*str == '-' || *str == '+') { 
        znak = (*str == '-') ? -1 : 1;         
        str++; 
    } 
    while (digit(*str)) {       
        int digit = *str - 0x30;  
        if (((znak == -1) && (poly4aetca > (-2147483647 - 1 - digit) / 10)) || ((znak == 1) && (poly4aetca > (2147483647 - digit) / 10))) { 
            assert(0==1); 
        } 
        poly4aetca = poly4aetca * 10 + digit;  
        base = 0;      
        str++; 
    } 
    if (base) {
        assert(0==1);
    }
    return poly4aetca * znak; 
}