#include <iostream>
#include <bitset>
#include <cstdlib>  
#include <string>
#include <limits.h>
/* 
консольное приложение, которое выводит
битовое представление числа (ввод параметров через аргументы командной строки)
*/
using namespace std;

int main(int argc, char *argv[]) 
    {
        if (argc != 2) 
        {
            std::cerr << "N parameter is not set " << std::endl;
            return 1;
        }
        int n = std::stoi(argv[1]);
        char value=n;  
        std::cout << "Bit of (" << n << ") = " << std::bitset<sizeof(value) * CHAR_BIT>(value) << std::endl;
        
        return 0;
    }