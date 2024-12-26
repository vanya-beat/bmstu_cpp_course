#include <iostream>
#include <string>
using namespace std;
 /* 
Создать программу, которая принимает размер массива от пользователя,
создает массив заданного размера, заполняет случайными его числами от 1
до N − 1 и выводит содержимое массива, через запятую в таком формате.
[1, 2, 666, 3234, 2]
*/ 
int main()
{
    int N; /* число элементов массива */
    std::cout << "Input elements number: ";
    std::cin >> N;
    int Arr[N];
    std::cout << "Array :" << std::endl;
    for (int i=0; i<N; i++)
    {
        int x = rand() % (N-1) + 1;
        Arr[i]=x;
        std::cout << (i==0 ? "[" : " ") << x << (i<(N-1)? "," : "]") ;
    };
    return 0;
}