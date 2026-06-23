#pragma once

#include <exception>
#include <iostream>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
	public:
	stack() : data_stack(nullptr), size_stack(0), capacity_stack(0) {} //создаём пустой стэк, не выделяем память

	bool empty() const noexcept { return size_stack == 0; } // проверяем пустой ли стэк

	size_t size() const noexcept { return size_stack; } // проверка размера стэка

	~stack() { //диструктор
		clear();
		::operator delete(data_stack); //очищаем память без вызова диструктора
	}

	template <typename... Args>
	void emplace(Args&&... args) {
		if (size_stack == capacity_stack) { //проверяем можем ли мы добавить элемент в текущий стэк
			//увеличиваем capacity
			size_t new_capacity_stack = 0;
			if (capacity_stack == 0) { 
				new_capacity_stack = 1;
			} 
			else {
				new_capacity_stack = capacity_stack + 1;
			}
		T* new_data_stack = (T*)::operator new(new_capacity_stack * sizeof(T)); //создаём новый массив
		for (size_t i = 0; i < size_stack; ++i) { // перемещаем все элементы в новый массив
			new (new_data_stack + i) T(std::move(data_stack[i])); //перемещение
			(data_stack + i)->~T(); //удаление старых эл. в data_stack
		}
		::operator delete(data_stack); //удаление указателя

		data_stack = new_data_stack;
		capacity_stack = new_capacity_stack;
		}
		new (data_stack + size_stack) T(std::forward<Args>(args)...);
    	++size_stack;
  	}

  // copy semantics (lvalue)
  	void push(const T& value) {
		if (size_stack == capacity_stack) { //проверяем можем ли мы добавить элемент в текущий стэк
			//увеличиваем capacity
			size_t new_capacity_stack = 0;
			if (capacity_stack == 0) { 
				new_capacity_stack = 1;
			} 
			else {
				new_capacity_stack = capacity_stack + 1;
			}
			T* new_data_stack = (T*)::operator new(new_capacity_stack * sizeof(T)); //создаём новый массив
			for (size_t i = 0; i < size_stack; ++i) { // перемещаем все элементы в новый массив
				new (new_data_stack + i) T(std::move(data_stack[i])); //перемещение
				(data_stack + i)->~T(); //удаление старых эл. в data_stack
			}
			::operator delete(data_stack); //удаление указателя

			data_stack = new_data_stack;
			capacity_stack = new_capacity_stack;
		}
		new (data_stack + size_stack) T(value); //с помощью конструктора для типа Т присваиваем сырой памяти value
		++size_stack;
  	}

  	// move semantics (rvalue)
	void push(T&& value) {
		if (size_stack == capacity_stack) { //проверяем можем ли мы добавить элемент в текущий стэк
			//увеличиваем capacity
			size_t new_capacity_stack = 0;
			if (capacity_stack == 0) { 
				new_capacity_stack = 1;
			} 
			else {
				new_capacity_stack = capacity_stack + 1;
			}
			T* new_data_stack = (T*)::operator new(new_capacity_stack * sizeof(T)); //создаём новый массив
			for (size_t i = 0; i < size_stack; ++i) { // перемещаем все элементы в новый массив
				new (new_data_stack + i) T(std::move(data_stack[i])); //перемещение
				(data_stack + i)->~T(); //удаление старых эл. в data_stack
			}
			::operator delete(data_stack); //удаление указателя

			data_stack = new_data_stack;
			capacity_stack = new_capacity_stack;
		}
		new (data_stack + size_stack) T(std::move(value)); //с помощью move записываем новым элементом value
		++size_stack;
  	}

	void clear() noexcept {
		while (size_stack > 0) {
			--size_stack; // уменьшаем переменную на 1
			(data_stack + size_stack)->~T(); //удаляем каждый элемент стэка начиная с конца(data_stack - указатель на начало массива, ~T() - диструктор типа T)
		}
	}

	void pop() {
		if (empty()) {
			throw std::underflow_error("Stack is empty!");
		}
		--size_stack;
		(data_stack + size_stack)->~T();
	}

	T& top() {
		if (empty()) {
			throw std::underflow_error("Stack is empty!");
		}
		return data_stack[size_stack - 1];
	}

	const T& top() const {
		if (empty()) {
			throw std::underflow_error("Stack is empty!");
		}
		return data_stack[size_stack - 1];
	 }

   	private:
	T* data_stack; //указатель на начало стэка
	size_t size_stack; //размер стэка
	size_t capacity_stack; //максимальная ёмкость стэка
};
}  // namespace bmstu
