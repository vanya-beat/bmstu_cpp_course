#include <iostream>
#include <string>
using namespace std;
 /* 
Заполнить массив размером N (вводится с клавиатуры) структур
Users (имя и возраст) и вывести на экран всех пользователей старше 18 лет.
*/
struct User {
    User() {  //конструктор
        std::cout << "User\n";
    }
    ~User() {    // деструктор
        std::cout << "~User\n";
    }
std::string name ;
int age ;
};
int main()
{
    /*User s; */
    User *str = new User; 
    delete str;
    return 0;
}