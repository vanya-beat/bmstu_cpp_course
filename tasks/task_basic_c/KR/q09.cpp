#include <iostream>
#include <string>
#include <cstdlib>  
using namespace std;
 /* 
Создать консольное приложение, которое выводит
на экран значение операции a op b (ввод параметров через аргументы команд-
ной строки. Первый параметр число, второй символ (+ или − или / или ∗),
третий второе число)
*/ 
int main (int argc, char *argv[]) {
     if (argc < 4) 
     {
            std::cerr << "There are no parameters!" << std::endl;
            return 1;
     };
    float a = std::stof(argv[1]);     /* 1-е число*/
    char op = * argv[2];            /* оператор */
    float b = std::stof(argv[3]);    /* 2-е число */
    switch (op) {
        case '+': 
            std::cout << "a + b = " << a+b << std::endl;
            break;
        case '-': 
            std::cout << "a - b = " << a-b << std::endl;
            break;
        case 'x':  {
            float k = a*b;
            std::cout << "a * b = " << k << std::endl;
            break;
        }
        case '/': {
            if (b==0) {
                std::cout << "Division by zero!" << std::endl;
            }
            else {
                float r = a/b;
                std::cout << "a / b = " << a << "/" << b << " = "  << r << std::endl;
            }
            break;
        }
        default:
            std::cout << "Uncorrect operator!"  << std::endl;
            break;
    };
    return 0;
}