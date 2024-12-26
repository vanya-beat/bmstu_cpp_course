#include <iostream>
/*
Написать объект буффер на 1024 байта, который можно перемещать, копировать
*/ 
using namespace std;
 
class Buffer {
private:
    int*    _data;          // Хранимые данные.
    int     _size;          // Размер  буффера.
    int     _freeSpace;     // Свободное место в буффере.
    int     _occupiedSpace; // Занятое мето.
 
public:
    Buffer(); /* конструктор */
    ~Buffer(); /* деструктор */
 
    // Метод, возвращающий свободное место.
    int GetFreeSpace()      
    { return _size - _occupiedSpace; }
 
    // Метод, возвращающий занятое место.
    int GetOccupiedSpace()  
    { return _occupiedSpace; }
 
    // Метод добавления элемента в буффер.
    void Add(int val);
 
    // Метод получения элемента из очереди
    int Get();
};
Buffer::Buffer()
{
    // Инициализация мета под буффер.
    _size = 1024;
    _data = new int[_size];
    _freeSpace = _size;
    _occupiedSpace = 0;
}
 
Buffer::~Buffer()
{ delete[] _data; } 
 
void Buffer::Add(int val)
{ _data[_occupiedSpace++ % _size] = val; }  
 
int Buffer::Get()
{
    // Проверка на пустоту буффера.
    if (_occupiedSpace == 0)
        throw "Buffer is empty";    
 
    int res = _data[0]; // Запоминаем 1 в очереди.
    // Сдвигаем очередь на одну позицию вперед.
    for (int i = 1; i < _occupiedSpace; ++i)
        _data[i - 1] = _data[i];
    // Меняем размер.
    --_occupiedSpace;
    return res; // Возвращаем первый элемент
}

int main()
{
    // Создаем экземпляр класса
    Buffer c;
    int count = 10;
 
    // Заполним очередь от 1 до 10 с выводом текущего стояния буффера.
    for (int i = 1; i <= count; ++i) {
        c.Add(i);
        cout << "Free space: " << c.GetFreeSpace() << "; Occupied space: " << c.GetOccupiedSpace() << endl;
    }
 
    // Очистка очереди от 1 до 10 с выводом текущего стояния буффера.
    for (int i = 0; i < count; ++i) {
        cout << "Get next: " << c.Get() << endl;
        cout << "Free space: " << c.GetFreeSpace() << "; Occupied space: " << c.GetOccupiedSpace() << endl;
    }
 
    return 0;
}
 
