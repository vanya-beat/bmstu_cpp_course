#include <iostream>
using namespace std;
/* Реализуйте шаблонный класс my_array , 
который содержит массив элементов, и методы для работы с ним */
template<typename T, size_t N>
class my_array 
{
public:
    /* размер массива */
    constexpr size_t size() const{
        return N;
    }
    /* элемент массива */
    T& operator[](size_t index){
        return mArray[index];
    }
    const T& operator[](size_t index) const{
        return mArray[index];
    }
    /* начало массива */
    T* begin() {
        return mArray;
    }
    const T* begin() const{
        return mArray;
    }
    /* конец массива */
    T* end() {
        return mArray+N;
    }
    const T* end() const{
        return mArray+N;
    }
private:
    T mArray[N]; //массив типа Т из N элементов
};

int main()
{
    my_array<int, 10> arr;      // массив целых чисел из 10 элементов
    for(size_t i = 0; i<arr.size(); i++){   // заполнение массива
        arr[i] = i;
    }
    std::cout << "my_array:" << std::endl;
    for(auto e: arr){
        std::cout << e << std::endl;
    }
    return 0;
}