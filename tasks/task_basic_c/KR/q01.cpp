#include <iostream>
#include <cstdlib>  
#include <algorithm>
/* приложение, которое выводит четные числа от m до n  */
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Not initialized parameters m and n " << std::endl;
        return 1;
    }

    int m = std::stoi(argv[1]);
    int n = std::stoi(argv[2]);
    /*
    std::cout << "Введите m " ;
    std::cin >> m;
    std::cout << "Введите n " ;
    std::cin >> n;*/
    for (int i = m; i < n+1; i++) {
        if (i%2==0)
        {
        std::cout << i << std::endl;
        };
    }
    return 0;
}