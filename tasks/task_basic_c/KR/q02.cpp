#include <iostream>
#include <cstdlib>  
#include <string>
/* 
консольное приложение, которое выводит
все простые числа от m до n (ввод параметров через аргументы командной строки)
*/
using namespace std;

bool isPrime(int n) // функция определения простого числа
{
  
    // для 1 возвращаем false
    if (n == 1 )
        return false;

    // проверяем все числа от 2 до n-1
    for (int i = 2; i < n; i++) {
      
        // если число делится на i без остатка, возвращаем false,
        if (n % i == 0)
            return false;
    }
  
      // не нашли деления без остатка
      return true;
}

int main(int argc, char *argv[]) 
    {
        if (argc != 3) 
        {
            std::cerr << "The m and n parameters are not set " << std::endl;
            return 1;
        }
        int m = std::stoi(argv[1]);
        int n = std::stoi(argv[2]);
        /*
        std::cout << "Введите m " ;
        std::cin >> m;
        std::cout << "Введите n " ;
        std::cin >> n;*/
        for (int i = m; i <= n; i++) 
        {
            if (isPrime(i))
                cout << i << std::endl;
        }
        return 0;
    }
