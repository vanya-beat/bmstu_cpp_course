#include <iostream>
#include <string>
using namespace std;
 /* 
Заполнить массив размером N (вводится с клавиатуры) структур
Users (имя и возраст) и вывести на экран всех пользователей старше 18 лет.
*/
struct User {
std :: string name ;
int age ;
};
int main()
{
    int N; /* число элементов массива */
    std::cout << "Input elements number: ";
    std::cin >> N;
    User Arr[N];
    for (int i=0;i<N; i++)  /* заполнение массива */
    {
        int age1;
        std::string name1;
        std::cout << "Input name and age [" << i << "]:";
        std::cin >> name1 >> age1;
        Arr[i].name=name1;
        Arr[i].age=age1;
    }
    std::cout << "Users older 18 years:" << std::endl;
    for (int i=0; i<N; i++)  /* вывод пользователей старше 18 лет */
    {
    if (Arr[i].age>18)
        {
            std::cout << Arr[i].name << ::endl;
        };
    }
    return 0;
}