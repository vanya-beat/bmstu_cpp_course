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
	if (data.empty()) {
		return;
	}
	head = new ForwardListNode(*data.begin());
	auto head_copy = head;
	for (auto it = data.begin() + 1; it != data.end(); ++it) 
	{
		auto tmp = new ForwardListNode(*it);
		head->next = tmp;
		head = head->next;
	}
	head = head_copy;
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
	os << "[";
	const ForwardListNode<T>* current = head;
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
	ForwardListNode<T>* current = head;
	while (current->next != nullptr) {
		if (current->data == current->next->data) {
			ForwardListNode<T>* duplicate = current->next;
			current->next = current->next->next;
			delete duplicate;
		}
		else {
			current = current->next;
		}
	}
}