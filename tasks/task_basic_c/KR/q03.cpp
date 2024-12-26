#include <iostream>
#include <cstdlib>  
#include <string>
/* 
консольное приложение, которое выводит
N первых чисел Фибоначчи (ввод параметров через аргументы командной строки)
*/
using namespace std;

int Fibonachi (int n) // функция определения n-го числа Fibonachi
{
    if (n<2)
        return n;
    int n1 = Fibonachi(n-1);
    int n2 = Fibonachi(n-2);
    int n3 = n1+n2;
    return n3;
}

int main(int argc, char *argv[]) 
    {
        if (argc != 2) 
        {
            std::cerr << "N parameters is not set " << std::endl;
            return 1;
        }
        int n = std::stoi(argv[1]);
        /*
        std::cout << "Введите N " ;
        std::cin >> n;*/
        for (int i = 0; i <= n; i++) 
        {
            cout << "i = " << i << " Fibonachi = " << Fibonachi(i) << std::endl;
        }
        return 0;
    }