#pragma once
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

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
	if (data.empty())
		return;

	head = new ForwardListNode<T>(data[0]);
	ForwardListNode<T>* current = head;

	for (size_t i = 1; i < data.size(); i++)
	{
		current->next = new ForwardListNode<T>(data[i]);
		current = current->next;
	}
}

template <typename T>
void delete_list(ForwardListNode<T>*& head)
{
	while (head != nullptr)
	{
		ForwardListNode<T>* temp = head;
		head = head->next;
		delete temp;
	}
}

template <typename T>
void print_list(const ForwardListNode<T>* head, std::ostream& os)
{
	os << "[";
	bool first = true;
	while (head != nullptr)
	{
		if (!first)
		{
			os << ", ";
		}
		os << head->data;
		first = false;
		head = head->next;
	}
	os << "]";
}

template <typename T>
void reverse_list(ForwardListNode<T>*& head)
{
	ForwardListNode<T>* prev = nullptr;
	ForwardListNode<T>* current = head;
	ForwardListNode<T>* next = nullptr;

	while (current != nullptr)
	{
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
	if (head == nullptr)
		return;

	ForwardListNode<T>* current = head;
	while (current->next != nullptr)
	{
		if (current->data == current->next->data)
		{
			ForwardListNode<T>* temp = current->next;
			current->next = current->next->next;
			delete temp;
		}
		else
		{
			current = current->next;
		}
	}
}