#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

template <typename T>
struct ForwardListNode
{
	T data;
	ForwardListNode* next = nullptr;
	ForwardListNode() = default;
	ForwardListNode(const T& data_) : data(data_) {}
	ForwardListNode(T&& data_) : data(std::move(data_)) {}
	ForwardListNode(const ForwardListNode& other) : data(other.data)
	{
		if (other.next)
		{
			next = new ForwardListNode(*other.next);
		}
	}
	ForwardListNode(ForwardListNode&& other) noexcept
		: data(std::move(other.data))
	{
		if (other.next)
		{
			next = other.next;
			other.next = nullptr;
		}
	}
	ForwardListNode(const T& data_, ForwardListNode* next_)
		: data(data_), next(next_)
	{
	}
	ForwardListNode(T&& data_, ForwardListNode* next_)
		: data(std::move(data_)), next(next_)
	{
	}
};

template <typename T>
void create_list(ForwardListNode<T>*& head, const std::vector<T>& data)
{
	head = nullptr;
	ForwardListNode<T>* tail = nullptr;
	if (data.empty()) {
		return;
	}

	try {
		for (const T& value : data) {
			ForwardListNode<T>* newNode = new ForwardListNode<T>{value, nullptr};
			if (newNode == nullptr) {
				throw std::bad_alloc();
			}

			if (head == nullptr) {
				head = newNode;
				tail = newNode;
			} else {
				tail->next = newNode;
				tail = newNode;
			}
		}

	} catch (const std::bad_alloc& e) {
		std::cerr << "Memory allocation failed: " << e.what() << std::endl;
		ForwardListNode<T>* current = head;
		while (current != nullptr) {
			ForwardListNode<T>* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;
		throw;
	}
}

template <typename T>
void delete_list(ForwardListNode<T>*& head)
{
	if (head == nullptr) {
		return;
	}
	ForwardListNode<T>* current = head;

	while (current != nullptr) {
		ForwardListNode<T>* next = current->next;
		delete current;
		current = next;
	}

	head = nullptr;
}

template <typename T>
void print_list(const ForwardListNode<T>* head, std::ostream& os)
{
	if (head == nullptr) {
		return;
	}

	const ForwardListNode<T>* current = head;
	os << "[";
	while (current != nullptr) {
		os << current->data;
		current = current->next;
		if (current == nullptr) {
			os << "]";
			break;
		}
		os << ", ";
	}
}

template <typename T>
void reverse_list(ForwardListNode<T>*& head)
{
	if (head == nullptr) {
		return;
	}
	if (head != nullptr && head->next == nullptr) {
		return;
	}

	ForwardListNode<T>* prev = nullptr;
    ForwardListNode<T>* current = head;
    ForwardListNode<T>* next = nullptr;

	while (current != nullptr) {
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	head = prev;
}

template <typename T>
void remove_duplicates(ForwardListNode<T>*& head)
{
	if (head == nullptr) {
		return;
	}
	if (head != nullptr && head->next == nullptr) {
		return;
	}

	ForwardListNode<T>* current = head;
	while (current->next != nullptr) {
		if (current->data == current->next->data) {
			ForwardListNode<T>* duplicate = current->next;
			current->next = current->next->next;
			delete duplicate;
		} else {
			current = current->next;
		}
	}
}